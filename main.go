package main

import (
	"fmt"
	"image"
	"image/draw"
	"os"

	"github.com/nfnt/resize"
	"golang.org/x/image/bmp"
)

func main() {
	f, _ := os.Open("./test.bmp")
	im, _ := bmp.Decode(f)
	im = resize.Resize(96, 103, im, resize.Lanczos3)
	result := image.NewGray(im.Bounds())
	draw.Draw(result, result.Bounds(), im, im.Bounds().Min, draw.Src)
	// var out [96][103]uint8
	for i := 0; i < im.Bounds().Dy(); i++ {
		for j := 0; j < im.Bounds().Dx(); j++ {
			c := result.GrayAt(j, i).Y
			_, _, _, a := im.At(j, i).RGBA()
			if a == 0 {
				fmt.Printf("255 ")
			} else {
				fmt.Printf("%d ", c)
			}
			// out[i][j] = c
			// if c < 100 {
			// 	fmt.Print("#")
			// } else {
			// 	fmt.Print(" ")
			// }
		}
	}
}
