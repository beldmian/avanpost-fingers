package core

import (
	"strconv"
	"strings"
)

func ParseMinutias(in string) [][3]int {
	splitted := strings.Split(in, " ")
	var out [][3]int
	for i := 0; i < len(splitted)-1; i += 3 {
		one, _ := strconv.Atoi(splitted[i])
		two, _ := strconv.Atoi(splitted[i+1])
		three, _ := strconv.Atoi(splitted[i+2])
		out = append(out, [3]int{one, two, three})
	}
	return out
}

func EncodeMinutias(in [][3]int) string {
	var out string
	for i := 0; i < len(in); i++ {
		out += strconv.Itoa(in[i][0]) + " "
		out += strconv.Itoa(in[i][1]) + " "
		out += strconv.Itoa(in[i][2]) + " "
	}
	return out
}
