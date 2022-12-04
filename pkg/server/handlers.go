package server

import (
	"bytes"
	"encoding/base64"
	"net/http"
	"runtime"
	"sync"

	"time"

	"github.com/beldmian/finger_checker/pkg/core"
	"github.com/labstack/echo/v4"
	"go.uber.org/zap"
	"golang.org/x/image/bmp"
)

type GetUserRequest struct {
	FingerprintImage string `json:"fingerprint_image"`
}

func (s *Server) getFingerprintUser(c echo.Context) error {
	start := time.Now()
	var body GetUserRequest
	if err := c.Bind(&body); err != nil {
		return c.JSON(http.StatusUnprocessableEntity, map[string]string{"error": err.Error()})
	}
	unbase, err := base64.StdEncoding.DecodeString(body.FingerprintImage)
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	im, err := bmp.Decode(bytes.NewReader(unbase))
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	fingerprint, err := s.r.Compute(im)
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	count, err := s.db.Fingerprint.GetFingerprintsCount()
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	var max float32 = 0
	var maxFp core.Fingerprint
	var maxMutex sync.Mutex
	var wg sync.WaitGroup
	wg.Add(runtime.NumCPU())
	for i := 0; i < runtime.NumCPU(); i++ {
		go func(i int) {
			for j := count * i / runtime.NumCPU(); j < count*(i+1)/runtime.NumCPU(); j += count / runtime.NumCPU() / 10 {
				fingerprints, _ := s.db.Fingerprint.GetBunchFingerprints(count/runtime.NumCPU()/10, j)
				for _, fprint := range fingerprints {
					val, _ := s.r.Compare(&fprint, fingerprint)
					maxMutex.Lock()
					if val > max {
						max = val
						maxFp = fprint
					}
					maxMutex.Unlock()
				}
			}
			wg.Done()
		}(i)
	}

	wg.Wait()
	s.l.Info("execution time", zap.String("time", time.Since(start).String()), zap.Float32("max", max))

	return c.JSON(200, maxFp)
}

type AddFingerprintRequest struct {
	FingerprintImage string `json:"fingerprint_image"`
	Name             string `json:"name"`
}

func (s *Server) addFingerprint(c echo.Context) error {
	start := time.Now()
	var body AddFingerprintRequest
	if err := c.Bind(&body); err != nil {
		return c.JSON(http.StatusUnprocessableEntity, map[string]string{"error": err.Error()})
	}
	unbase, err := base64.StdEncoding.DecodeString(body.FingerprintImage)
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	im, err := bmp.Decode(bytes.NewReader(unbase))
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	fingerprint, err := s.r.Compute(im)
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	fingerprint.Name = body.Name
	if _, err := s.db.Fingerprint.InsertFingerprint(fingerprint); err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{"error": err.Error()})
	}
	s.l.Info("execution time", zap.String("time", time.Since(start).String()))
	return c.JSON(http.StatusOK, map[string]bool{"ok": true})
}
