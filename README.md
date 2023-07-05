# Audio Chat Over Internet

[![License](https://img.shields.io/github/license/JorgeSC31/AudioChat)](https://github.com/JorgeSC31/AudioChat/blob/main/LICENSE)

Audio Chat Over Internet is a client-server console application developed in C++ that allows multiple users to communicate with each other from different parts of the world. The application utilizes UDP sockets and the PortAudio library to enable real-time audio communication over the internet.

This project aims to provide a simple and efficient solution for audio chatting, enabling users to establish audio connections with one another and engage in conversation, regardless of their physical locations. The application's client-server architecture allows users to connect to a central server, which facilitates the audio communication between multiple clients.

## Features

- Real-time audio communication: Users can transmit and receive audio data in real-time, enabling seamless conversations over the internet.
- Multiple user support: The application supports multiple concurrent users, allowing multiple participants to engage in audio conversations simultaneously.
- Client-server architecture: The server acts as a central hub, facilitating the communication between connected clients. Clients can connect to the server to join the audio chat.
- UDP sockets: The application utilizes UDP sockets for network communication, providing low-latency and efficient transmission of audio data.

## Prerequisites

- C++ compiler: The project requires a C++ compiler that supports C++11 or later versions.
- [Premake 5](https://premake.github.io): Premake 5 is used as the build system for this project.
- [PortAudio](http://www.portaudio.com): Install the PortAudio library to enable audio input and output.

## Getting Started

Follow these steps to get started with the Audio Chat Over Internet project:

1. Clone the repository:

   ```bash
   git clone https://github.com/JorgeSC31/AudioChat.git
   ```

2. Install the necessary dependencies:

   - Install Premake 5: Follow the instructions provided with Premake 5 to install it on your system.
   - Install PortAudio: Download and install the appropriate PortAudio library for your platform.

3. Build the project:

   - Generate the project files using Premake:

     ```bash
     cd AudioChat
     premake5 gmake
     ```

   - Build the project using Make:
     ```bash
     make config=release
     ```
4. Run the server:

   - Start the server application by running the generated server executable.
     ```bash
     ./bin/Release-linux-x86_64/Server/Server
     ```

5. Run the client:

   - Start the client application on each client machine by running the generated client executable.
     ```bash
     ./bin/Release-linux-x86_64/Client/Client
     ```
   - Provide the necessary connection details (such as server IP address and port) to connect the client to the server.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

- [PortAudio](http://www.portaudio.com/): Cross-platform audio I/O library.
- [Premake](https://premake.github.io/): Build configuration tool.

## Contributing

Contributions to the Audio Chat Over Internet project are welcome! If you encounter any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.
