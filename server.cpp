#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

// Raw String Literal for the complete Frontend (HTML/CSS/JS)
const std::string html_content = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>TypingSpeedPro | Ultimate Performance Test</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --primary: #6366f1;
            --secondary: #a855f7;
            --bg-dark: #0f172a;
            --glass-bg: rgba(30, 41, 59, 0.7);
            --glass-border: rgba(255, 255, 255, 0.1);
            --text-main: #f8fafc;
            --text-muted: #94a3b8;
            --success: #22c55e;
            --error: #ef4444;
        }

        * { margin: 0; padding: 0; box-sizing: border-box; }


        
        body {
            font-family: 'Outfit', sans-serif;
            background-color: var(--bg-dark);
            background-image: 
                radial-gradient(at 0% 0%, hsla(253,16%,7%,1) 0, transparent 50%), 
                radial-gradient(at 50% 0%, hsla(225,39%,30%,1) 0, transparent 50%), 
                radial-gradient(at 100% 0%, hsla(339,49%,30%,1) 0, transparent 50%);
            color: var(--text-main);
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            overflow: hidden;
        }

        .container {
            width: 90%;
            max-width: 900px;
            background: var(--glass-bg);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--glass-border);
            border-radius: 24px;
            padding: 3rem;
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
            animation: fadeIn 0.8s ease-out;
            position: relative;
            overflow: hidden;
        }

        /* Ambient glow effect */
        .container::before {
            content: '';
            position: absolute;
            top: -50%;
            left: -50%;
            width: 200%;
            height: 200%;
            background: radial-gradient(circle, rgba(99, 102, 241, 0.1) 0%, transparent 70%);
            pointer-events: none;
            z-index: -1;
        }

        h1 {
            font-size: 2.5rem;
            font-weight: 700;
            text-align: center;
            margin-bottom: 0.5rem;
            background: linear-gradient(135deg, #fff 0%, #cbd5e1 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        p.subtitle {
            text-align: center;
            color: var(--text-muted);
            margin-bottom: 2.5rem;
            font-size: 1.1rem;
        }

        .stats-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 1.5rem;
            margin-bottom: 2.5rem;
        }

        .stat-card {
            background: rgba(255, 255, 255, 0.05);
            padding: 1.5rem;
            border-radius: 16px;
            text-align: center;
            border: 1px solid rgba(255, 255, 255, 0.05);
            transition: transform 0.3s ease;
        }

        .stat-card:hover {
            transform: translateY(-5px);
            background: rgba(255, 255, 255, 0.08);
        }

        .stat-value {
            font-size: 2.5rem;
            font-weight: 700;
            color: var(--secondary);
            margin-bottom: 0.25rem;
        }
        
        .stat-label {
            font-size: 0.9rem;
            color: var(--text-muted);
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        #wpm-display { color: var(--primary); }
        #accuracy-display { color: var(--success); }

        .typing-area {
            position: relative;
            background: rgba(15, 23, 42, 0.6);
            border-radius: 16px;
            padding: 2rem;
            border: 1px solid var(--glass-border);
            min-height: 180px;
            margin-bottom: 2rem;
            font-size: 1.5rem;
            line-height: 1.8;
            font-family: 'Courier New', Courier, monospace;
            box-shadow: inset 0 2px 4px rgba(0,0,0,0.3);
        }

        #text-display {
            color: var(--text-muted);
            user-select: none;
        }

        .char { transition: color 0.1s; }
        .char.correct { color: var(--text-main); text-shadow: 0 0 8px rgba(255,255,255,0.4); }
        .char.incorrect { color: var(--error); text-decoration: underline; }
        .char.current { 
            background-color: var(--primary); 
            color: white; 
            border-radius: 2px;
            animation: pulse 1.5s infinite;
        }

        #input-field {
            position: absolute;
            opacity: 0;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            cursor: default;
        }

        .controls {
            display: flex;
            justify-content: center;
            gap: 1rem;
        }

        button {
            padding: 0.8rem 2.5rem;
            border-radius: 12px;
            border: none;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            font-family: inherit;
        }

        .btn-restart {
            background: linear-gradient(135deg, var(--primary), var(--secondary));
            color: white;
            box-shadow: 0 4px 12px rgba(99, 102, 241, 0.4);
        }

        .btn-restart:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 16px rgba(99, 102, 241, 0.6);
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(20px); }
            to { opacity: 1; transform: translateY(0); }
        }

        @keyframes pulse {
            0% { box-shadow: 0 0 0 0 rgba(99, 102, 241, 0.7); }
            70% { box-shadow: 0 0 0 6px rgba(99, 102, 241, 0); }
            100% { box-shadow: 0 0 0 0 rgba(99, 102, 241, 0); }
        }
        
        /* Mobile responsive */
        @media (max-width: 768px) {
            .stats-grid { grid-template-columns: 1fr; }
            .container { padding: 1.5rem; width: 95%; }
            h1 { font-size: 1.8rem; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>TypingSpeedPro</h1>
        <p class="subtitle">Test your typing performance with C++ powered precision.</p>

        <div class="stats-grid">
            <div class="stat-card">
                <div class="stat-value" id="wpm-display">0</div>
                <div class="stat-label">Words / Min</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="accuracy-display">100%</div>
                <div class="stat-label">Accuracy</div>
            </div>
            <div class="stat-card">
                <div class="stat-value" id="time-display">60</div>
                <div class="stat-label">Time Left</div>
            </div>
        </div>

        <div class="typing-area" onclick="focusInput()">
            <div id="text-display"></div>
            <input type="text" id="input-field" autocomplete="off">
        </div>

        <div class="controls">
            <button class="btn-restart" onclick="resetTest()">Restart Test</button>
        </div>
    </div>

    <script>
        const quotes = [
            "The quick brown fox jumps over the lazy dog.",
            "To be or not to be, that is the question.",
            "All that glitters is not gold.",
            "Performance measurement is a cornerstone of computer science.",
            "C++ is a high-performance language traditionally associated with system-level tasks.",
            "Modern software demands interactivity and network connectivity.",
            "Socket programming bridges the gap between low-level logic and high-level interfaces.",
            "Practice makes perfect in the art of typing and coding."
        ];

        let timeLeft = 60;
        let timeElapsed = 0;
        let timer = null;
        let isRunning = false;
        let currentQuote = "";
        let charIndex = 0;
        let mistakes = 0;

        const wpmDisplay = document.getElementById('wpm-display');
        const accuracyDisplay = document.getElementById('accuracy-display');
        const timeDisplay = document.getElementById('time-display');
        const textDisplay = document.getElementById('text-display');
        const inputField = document.getElementById('input-field');

        function loadQuote() {
            currentQuote = quotes[Math.floor(Math.random() * quotes.length)];
            textDisplay.innerHTML = "";
            currentQuote.split('').forEach(char => {
                const span = document.createElement("span");
                span.innerText = char;
                span.classList.add("char");
                textDisplay.appendChild(span);
            });
            textDisplay.firstChild.classList.add("current");
        }

        function focusInput() {
            inputField.focus();
        }

        function initTyping() {
            const chars = textDisplay.querySelectorAll(".char");
            let typedChar = inputField.value.split('')[charIndex];

            if (!isRunning && inputField.value.length > 0) {
                startTimer();
            }

            if (timer && timeLeft > 0) {
                if (charIndex < chars.length) {
                    if (typedChar == null) { // Backspace
                        charIndex--;
                        if(charIndex < 0) charIndex = 0;
                        chars[charIndex].classList.remove("correct", "incorrect");
                        updateCursor(charIndex);
                    } else {
                        if (chars[charIndex].innerText === typedChar) {
                            chars[charIndex].classList.add("correct");
                        } else {
                            chars[charIndex].classList.add("incorrect");
                            mistakes++;
                        }
                        charIndex++;
                        if (charIndex < chars.length) updateCursor(charIndex);
                        else {
                            // Finished quote, load new one? For simple test, just stop or loop.
                            // Let's loop to a new quote but keep timer running
                           loadNewQuoteKeepingStats();
                        }
                    }
                }
            }
            
            // Calculate Stats
            let wpm = Math.round(((charIndex - mistakes) / 5) / (timeElapsed / 60));
            wpm = wpm < 0 || !isFinite(wpm) ? 0 : wpm;
            wpmDisplay.innerText = wpm;
            
            let accuracy = Math.round(((charIndex - mistakes) / charIndex) * 100);
            accuracy = accuracy < 0 || !isFinite(accuracy) ? 100 : accuracy;
            accuracyDisplay.innerText = accuracy + "%";
        }

        function updateCursor(index) {
            const chars = textDisplay.querySelectorAll(".char");
            chars.forEach(span => span.classList.remove("current"));
            if(index < chars.length) chars[index].classList.add("current");
        }
        
        function loadNewQuoteKeepingStats() {
            // Complex logic to append new text or reset visual but keep stats
            // For simplicity in this demo, just reset input buffer but keep stats flowing?
            // Easier: Just end test if they finish the sentence for MVP. 
            // Or reload quote.
            inputField.value = "";
            loadQuote();
            charIndex = 0;
        }

        function startTimer() {
            if(isRunning) return;
            isRunning = true;
            timer = setInterval(() => {
                if (timeLeft > 0) {
                    timeLeft--;
                    timeElapsed++;
                    timeDisplay.innerText = timeLeft;
                    
                    // Live WPM update
                    let wpm = Math.round(((charIndex - mistakes) / 5) / (timeElapsed / 60));
                    wpm = wpm < 0 || !isFinite(wpm) ? 0 : wpm;
                    wpmDisplay.innerText = wpm;
                } else {
                    clearInterval(timer);
                    inputField.disabled = true;
                }
            }, 1000);
        }

        function resetTest() {
            clearInterval(timer);
            timeLeft = 60;
            timeElapsed = 0;
            charIndex = 0;
            mistakes = 0;
            isRunning = false;
            inputField.disabled = false;
            inputField.value = "";
            timeDisplay.innerText = timeLeft;
            wpmDisplay.innerText = 0;
            accuracyDisplay.innerText = "100%";
            loadQuote();
            focusInput();
        }

        inputField.addEventListener("input", initTyping);
        loadQuote();
        document.addEventListener('keydown', () => inputField.focus());
    </script>
</body>
</html>
)HTML";

void start_server() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Port 8080 might be in use.\n";
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    std::cout << "-------------------------------------------\n";
    std::cout << " TypingSpeedPro Server Running\n";
    std::cout << " Port: 8080\n";
    std::cout << " URL:  http://localhost:8080\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "Press Ctrl+C to stop the server.\n";

    while (true) {
        SOCKET new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed.\n";
            continue;
        }

        char buffer[1024] = {0};
        recv(new_socket, buffer, 1024, 0);
        
        // Construct HTTP Response
        // Including Content-Length is good practice
        std::string response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + std::to_string(html_content.size()) + "\r\n"
            "Connection: close\r\n\r\n" + 
            html_content;

        // Send the response fully
        size_t total_sent = 0;
        const char* data_ptr = response.data();
        size_t remaining = response.size();

        while (remaining > 0) {
            int sent = send(new_socket, data_ptr + total_sent, remaining, 0);
            if (sent == SOCKET_ERROR) {
                std::cerr << "Send failed.\n";
                break;
            }
            total_sent += sent;
            remaining -= sent;
        }

        // Graceful shutdown
        shutdown(new_socket, SD_SEND);
        closesocket(new_socket);
    }

    closesocket(server_fd);
    WSACleanup();
}

int main() {
    start_server();
    return 0;
}
