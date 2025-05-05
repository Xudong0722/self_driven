import requests

def send_request(method, url, data=None):
    try:
        headers = {
            'Content-Type': 'application/json',
            'Authorization': 'Bearer example-token'
        }

        if method == 'GET':
            response = requests.get(url, headers=headers, verify=False)  # verify=False 跳过证书验证
        elif method == 'POST':
            response = requests.post(url, headers=headers, json=data, verify=False)

        print(f"Response Status: {response.status_code}")
        print(f"Response Body: {response.text}")
    except requests.exceptions.RequestException as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    server_url = "https://localhost:8443"

    print("Sending GET request...")
    send_request("GET", server_url)

    print("\nSending POST request...")
    send_request("POST", server_url, data={"name": "Python HTTPS learner"})
