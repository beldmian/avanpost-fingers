package recognizer

import (
	"fmt"
	"image"
	"image/draw"
	"unsafe"

	"github.com/beldmian/finger_checker/pkg/core"
	"github.com/nfnt/resize"
)

/*
#include "main.h"
*/
import "C"

type Recognizer struct{}

func (r *Recognizer) Compute(im image.Image) (*core.Fingerprint, error) {
	fmt.Println(im.Bounds().Dx())
	if im.Bounds().Dx() != 96 {
		im = resize.Resize(96, 103, im, resize.Lanczos3)
	}
	pixels := new([103][96]C.short)
	out := new([5000][3]C.short)

	result := image.NewGray(im.Bounds())
	draw.Draw(result, result.Bounds(), im, im.Bounds().Min, draw.Src)

	for i := 0; i < 96; i++ {
		for j := 0; j < 103; j++ {
			pixels[j][i] = C.short(result.GrayAt(j, i).Y)
			_, _, _, a := im.At(j, i).RGBA()
			if a == 0 {
				pixels[j][i] = 255
			}
		}
	}
	out_len := C.algorithm((*[96]C.short)(unsafe.Pointer(&pixels[0])), (*[3]C.short)(unsafe.Pointer(&out[0])))
	var minutias [][3]int
	for i := 0; i < int(out_len); i++ {
		minutias = append(minutias, [3]int{int(out[i][0]), int(out[i][1]), int(out[i][2])})
	}
	return &core.Fingerprint{
		ID:       -1,
		Name:     "",
		Minutias: minutias,
	}, nil
}

func (r *Recognizer) Compare(fp1 *core.Fingerprint, fp2 *core.Fingerprint) (float32, error) {
	var output C.double
	if len(fp1.Minutias) == 0 || len(fp2.Minutias) == 0 {
		return -1, nil
	}
	if len(fp1.Minutias) < len(fp2.Minutias) {
		output = C.compare((*[3]C.short)(unsafe.Pointer(&fp1.Minutias[0])), C.int(len(fp1.Minutias)),
			(*[3]C.short)(unsafe.Pointer(&fp2.Minutias[0])), C.int(len(fp2.Minutias)))
	} else {
		output = C.compare((*[3]C.short)(unsafe.Pointer(&fp2.Minutias[0])), C.int(len(fp2.Minutias)),
			(*[3]C.short)(unsafe.Pointer(&fp1.Minutias[0])), C.int(len(fp1.Minutias)))
	}
	return float32(output), nil
}

func Provide() *Recognizer {
	return &Recognizer{}
}
