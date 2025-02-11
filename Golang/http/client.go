package main

import (
    "bytes"
    "fmt"
    "io/ioutil"
    "net/http"
)

func sendRequest(method, url string, body []byte) {
    client := &http.Client{}

    // 创建请求
    req, err := http.NewRequest(method, url, bytes.NewBuffer(body))
    if err != nil {
        fmt.Println("Error creating request:", err)
        return
    }

    // 添加请求头
    req.Header.Add("Content-Type", "application/json")
    req.Header.Add("Authorization", "Bearer example-token")

    // 发送请求并获取响应
    resp, err := client.Do(req)
    if err != nil {
        fmt.Println("Error sending request:", err)
        return
    }
    defer resp.Body.Close()

    // 读取响应
    responseBody, _ := ioutil.ReadAll(resp.Body)
    fmt.Printf("Response status: %s\n", resp.Status)
    fmt.Printf("Response body: %s\n", string(responseBody))
}

func main() {
    url := "http://localhost:8080"

    fmt.Println("Sending GET request...")
    sendRequest("GET", url, nil)

    fmt.Println("\nSending POST request...")
    sendRequest("POST", url, []byte(`{"name": "Go learner"}`))
}
