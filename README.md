# Control Utils

**Control Utils** is a lightweight C library providing essential utilities for real-time control system development, particularly in embedded systems. It is designed to streamline the implementation of common control algorithms.

## Features

- **Controllers**: Flexible and easy-to-tune controllers, such as PID and hysteresis/bang-bang.
- **Z-Transform Implementation**: Implement discrete-time controller transfer functions in C with ease  
- **Filters**: Low-Pass Filtering (with plans to add Moving Average Filtering)
- **Modular Design**: Easily integrate specific components without overhead.

## Repository Structure

- `control_utils_lib/`: Core library files in C.
- `scripts/`: Scripts for building and testing the repository.

## Integrating into a project
- Import this repository as a submodule
- Point your build system to the paths inside `control_utils_lib`

## Contributing

Contributions are welcome! Submit a pull request with your enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
