## Building

### Hardware (RP2040)
platformio run -e nanorp2040connect

### Native (Testing)
platformio run -e native

### Running Tests
platformio test -e native

### Build Configuration

This project uses a dual-environment setup in `platformio.ini`:

- `[env:nanorp2040connect]` - Builds for actual RP2040 hardware using Arduino framework
- `[env:native]` - A stub build environment for platform compatibility and testing

The `NATIVE_BUILD` preprocessor flag controls conditional compilation:
- When defined (`-DNATIVE_BUILD`), the build uses stub implementations instead of Arduino/hardware APIs
- This allows `platformio run -e native` to work without errors for CI/CD and development workflow verification

## TODO between the micro and the digital pot to 1ohm instead of 1k --> it currently halfes the output range of the digital pot.
- [ ] write back to the wheel the informations needed like the speed, time, satellites
- [ ] display the satellite info on the weel
- [ ] reduce the sensibility of the encoders
- [ ] add the RPM --> from the engine bay diagnostic connector
- [ ] fix the ledbar
- [ ] add the possibility to change the time for the potentiometer output from the weel

## Repository Notes

- **.gitignore updates**: Added entries to ignore IDE configuration files (`.vscode/*`), environment files (`.env`, `.env.*`), and credential files (`*.pem`, `*.key`, `credentials`, `secrets`). This helps keep sensitive data out of the repository.
- **Test suite status**: Unit tests are located in the `test/` directory and can be executed with `platformio test -e native`. The native test environment provides stub implementations, ensuring tests run in CI without hardware.
