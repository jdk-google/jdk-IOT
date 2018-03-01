package main

import "fmt"
import "github.com/yryz/ds18b20"
import "strconv"
import "net/http"
import "time"
import "os"
import "os/exec"
import "strings"

/*
  file_content, err := ioutil.ReadFile("/sys/bus/w1/devices/" + device_id + "/w1_slave")
  if err != nil { return 0, err }
  temp_string := string(file_content[len(file_content)-8:])
  fmt.Println("Temperature Received: "+ temp_string)
  fmt.Println(temp_string[0:2])
  if temp_string[0:2] != "t=" { 
    return 0.0, errors.New("Unexpected file content: "+ string(file_content) ) 
  }
  fmt.Println(strconv.Atoi(temp_string[2:5]))
*/

func GetSSID() string {
  cmd := exec.Command ("/sbin/iwgetid", "-r")
  output, err := cmd.Output()
  if (err != nil) {
    fmt.Println("Error Executing command")
    fmt.Println(err)
  }
  return strings.TrimSpace(string(output))
}

func ReadTermometers()  {
  ssid := GetSSID()
  hostname, _ := os.Hostname()
  fmt.Println("Detected Network:" + ssid)
  sensors, err := ds18b20.Sensors()
  if err != nil {
    fmt.Println("Error reading sensor from library")
    fmt.Println(err)
  }
  fmt.Printf("sensor IDs: %v\n", sensors)
  fmt.Println(time.Now().Unix())

  for _, sensor := range sensors {
      t, err := ds18b20.Temperature(sensor)
      if err == nil {
          fmt.Printf("sensor: %s temperature: %.2f°C\n", sensor, t)
          url := "http://testowy-projeklt.appspot.com/saveTemperature?"+
                 "temp=" + strconv.FormatFloat(t, 'f', 2, 64)+
                 "&device_id=" + sensor +
                 "&timestamp=" + strconv.FormatInt(time.Now().Unix(), 10)+
                 "&ssid="+ssid+
                 "&hostname="+hostname
          fmt.Println("Connecting to " + url)
          response, err := http.Get(url)
          if err!= nil {
            fmt.Println ("Error Making HTTP Request")
            fmt.Println (err)
          }
          fmt.Println(response)
      }
  }

  return
}

func main () {
  fmt.Println("Welcom to Termometer")
  fmt.Println("This is time: " + time.Now().Format(time.UnixDate))
  ReadTermometers()
}

