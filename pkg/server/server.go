package server

import (
	"context"

	"github.com/beldmian/finger_checker/pkg/config"
	"github.com/beldmian/finger_checker/pkg/database"
	"github.com/beldmian/finger_checker/pkg/recognizer"
	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
	"go.uber.org/fx"
	"go.uber.org/zap"
)

type Server struct {
	e    *echo.Echo
	l    *zap.Logger
	db   *database.Database
	r    *recognizer.Recognizer
	addr string
}

func Provide(lc fx.Lifecycle, l *zap.Logger, c *config.Config, db *database.Database, r *recognizer.Recognizer) *Server {
	s := Server{
		e:    echo.New(),
		l:    l,
		db:   db,
		r:    r,
		addr: c.Addr,
	}
	s.e.HideBanner = true
	s.e.HidePort = true
	s.e.Debug = false
	s.e.Use(middleware.CORSWithConfig(middleware.CORSConfig{
		AllowOrigins: []string{"*"},
		AllowHeaders: []string{"*"},
	}))
	s.e.POST("/get_owner", s.getFingerprintUser)
	s.e.POST("/add_fingerprint", s.addFingerprint)
	lc.Append(fx.Hook{
		OnStart: func(ctx context.Context) error {
			go s.Start()
			return nil
		},
		OnStop: func(ctx context.Context) error {
			return nil
		},
	})
	return &s
}

func (s *Server) Start() error {
	s.l.Info("starting server", zap.String("port", s.addr))
	s.e.Start(s.addr)
	return nil
}
