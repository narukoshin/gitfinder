# Gitfinder

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![C++](https://img.shields.io/badge/C%2B%2B-Main-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)


![Hehe](https://media.tenor.com/3a2uXqYZMWUAAAAj/anime-good.gif)
![Ayaya](https://media.tenor.com/wxhwrp30VkgAAAAj/ayaya-club-ayaya.gif)

<span>A scanning tool for finding website domains with exposed `.git` folders that can be used to obtain the full source code of the website.</span>

## Features
- Multi-threaded scanning for efficient performance.
- Customizable request timeouts and headers.
- Reads target URLs from a configurable file (e.g., `urls.txt`).
- Supports building for Linux and Windows platforms.
- Lightweight and easy to configure.

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/narukoshin/gitfinder.git
   cd gitfinder
   ```

2. Ensure you have the required build tools installed:
   - CMake (version 3.10 or higher)
   - Ninja
   - For Windows cross-compilation: MinGW-w64 (optional, if using toolchains)

3. Build and install (recommended for local use):
   ```
   make && sudo make install
   ```
   This will build the tool using CMake and Ninja, and install it to `/usr/local/bin/gitfinder`. You can customize with variables like `BUILD_TYPE=Debug`, `TOOLCHAIN=toolchains/linux-x86_64.cmake`, or `PREFIX=/custom/path`.

   For cross-platform builds and archives, use the build script:
   ```
   ./build.sh
   ```
   This compiles for Linux (amd64) and Windows (amd64), creating `.tar.gz` archives in the `releases/` directory.

4. Extract the appropriate archive if using `./build.sh` (e.g., `gitfinder-1.0.0-linux-amd64.tar.gz`) and use the `gitfinder` binary.

## Collecting Domains

The repository includes `collector.py`, a Python script that crawls Google to collect domain names and generate the `urls.txt` file used by GitFinder.

**Requirements**: Python 3.x. Install any necessary libraries (e.g., `requests`, `beautifulsoup4`) via `pip3 install` if not already available.

**Usage**:
```
python3 collector.py
```
Run the script with appropriate options to search Google for domains and output them to `urls.txt`.

## Configuration

1. Copy the sample configuration file:
   ```
   cp config.sample.yml config.yml
   ```

2. Edit `config.yml` to suit your needs:
   - `threads`: Number of concurrent threads (e.g., 8).
   - `timeout`: Request timeout (e.g., 30s).
   - `collection_file`: Path to the file containing target URLs (one URL per line, e.g., `urls.txt`).
   - `headers`: Custom HTTP headers (e.g., User-Agent for spoofing).

   Example `config.yml`:
   ```yaml
   threads: 8
   timeout: 30s
   collection_file: urls.txt
   headers:
     "User-Agent": "Mozilla/5.0 (compatible; GitFinder/1.0)"
   ```

3. Prepare your `urls.txt` file with a list of target domains (one per line). Use `collector.py` or generate manually.

## Usage

Run the tool with the configured settings:
```bash
./gitfinder
```

The tool will scan the URLs listed in your collection file for exposed `.git` folders and report findings.

## Contributing

Contributions are welcome! Feel free to submit pull requests for improvements, bug fixes, or new features.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

<p align="center"><img src="https://media.tenor.com/K5cpoaTsn5wAAAAi/anime.gif"></p>
<p align="center"><span><b>ー Project by THE NEET FAMILY ー</b><br><small>We think. We create. We produce.</small></span></p>
