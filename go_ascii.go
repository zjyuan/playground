package main

import (
	"fmt"
	"strings"
	"os/exec"
	"bytes"
)

func main(){
	c := 31
	str := string(c)
	uuid := strings.Join([]string{"image","%d"}, str)
	fmt.Println(uuid)
	cmd := "ffmpeg -i /media/luis/d/rtsp/2.mp4 -vf fps=1 -q:v 2 "+uuid
	fmt.Println(cmd)

	output, _ := exec_shell(cmd)
	fmt.Println(output)
}

func exec_shell(s string) (string, error){
    cmd := exec.Command("/bin/bash", "-c", s)

    var out bytes.Buffer
    //cmd.Stdout = &out
    cmd.Stderr = &out

    err := cmd.Run()

    return out.String(), err
}