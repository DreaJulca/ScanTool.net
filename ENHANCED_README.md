# ScanTool.net Enhanced Edition

## Enhanced Automotive Diagnostic Features

This enhanced version of ScanTool.net includes advanced automotive diagnostic and programming capabilities designed for modern vehicles and professional use.

### New Features Added

#### 1. **ECU Programming & Calibration**
- **Injector Coding**: Program injector flow rate codes for proper fuel delivery
- **Throttle Body Calibration**: Learn throttle position sensor limits and calibration
- **Steering Angle Sensor Calibration**: Set steering wheel center position and sensor zero point
- **Battery Registration**: Register new battery specifications with Battery Management System (BMS)
- **Electronic Parking Brake (EPB) Service Reset**: Retract EPB calipers for brake service
- **ECU Firmware Updates**: Integration point for custom ECU firmware programming

#### 2. **Advanced Real-time Data Visualization**
- **Multi-chart Dashboard**: Display up to 16 simultaneous data visualizations
- **Chart Types**: Line charts, gauges, bar charts, and digital displays
- **Real-time Recording**: Save sensor data to CSV files for analysis
- **Custom Layouts**: Pre-configured layouts for engine, transmission, and emissions monitoring
- **Alert System**: Configurable warnings for parameter thresholds
- **Playback Mode**: Review recorded diagnostic sessions

#### 3. **Enhanced Diagnostic Procedures**
- **DPF Regeneration**: Force diesel particulate filter regeneration cycles
- **Transmission Adaptation**: Reset and relearn transmission parameters
- **ABS System Bleeding**: Guided ABS brake bleeding procedures
- **Airbag System Calibration**: Reset airbag system after component replacement

#### 4. **Maintenance Service Resets**
- **Engine Oil Service**: Reset oil change intervals
- **Transmission Fluid**: Reset transmission service intervals  
- **Brake Fluid Service**: Reset brake fluid change intervals
- **Coolant System**: Reset coolant service intervals
- **Air Filter**: Reset air filter replacement intervals
- **Spark Plugs**: Reset spark plug service intervals
- **Timing Belt**: Reset timing belt service intervals

#### 5. **Enhanced Communication Protocols**
- **UDS (Unified Diagnostic Services)**: ISO 14229 support for modern vehicles
- **KWP2000**: Enhanced Keyword Protocol 2000 implementation
- **ISO-TP**: Multi-frame message support for large data transfers
- **Security Access**: Seed/key authentication for protected ECU functions
- **Session Management**: Proper diagnostic session control

### Technical Implementation

#### New Modules
- `ecu_programming.c/h`: ECU programming and calibration functions
- `realtime_charts.c/h`: Advanced data visualization engine
- `advanced_diagnostics.c/h`: Specialized diagnostic procedures
- `serial_enhanced.c/h`: Enhanced communication protocol support
- `main_menu_enhanced.c/h`: Updated user interface

#### Protocol Support
- **OBD-II**: Original protocol support maintained
- **UDS over CAN**: ISO 14229-1 diagnostic services
- **KWP2000**: ISO 14230 keyword protocol
- **ISO-TP**: ISO 15765-2 transport protocol for CAN
- **Security**: Seed/key algorithms for ECU access

#### Data Visualization
- **Allegro Graphics**: Leverages existing graphics library
- **Double Buffering**: Smooth real-time chart updates
- **Configurable Refresh Rates**: 1Hz to 20Hz update rates
- **Auto-scaling**: Automatic Y-axis scaling for optimal display
- **Data Recording**: CSV export for external analysis

### Safety and Compatibility

#### Safety Features
- **Prerequisites Checking**: Verify vehicle state before procedures
- **Safety Warnings**: Multi-level safety alerts for high-risk procedures
- **Session Timeouts**: Automatic timeout for diagnostic sessions
- **Error Recovery**: Graceful handling of communication errors

#### Vehicle Compatibility
- **VIN Verification**: Check vehicle compatibility before procedures
- **ECU Detection**: Automatic ECU type and capability detection
- **Protocol Negotiation**: Automatic protocol selection and fallback
- **Manufacturer Extensions**: Support for manufacturer-specific functions

### Integration with Custom ECU Firmware

The enhanced ScanTool.net is designed to integrate with your custom ECU firmware project:

#### Firmware Programming Interface
- **Bootloader Communication**: Direct communication with ECU bootloaders
- **Flash Memory Programming**: Support for various flash memory types
- **Verification**: Checksum and CRC verification of programmed firmware
- **Version Management**: Track firmware versions and update history

#### Development Integration
- **Build Integration**: Direct integration with ECU build systems
- **Debug Interface**: Real-time debugging and parameter monitoring
- **Calibration Tools**: Live calibration parameter adjustment
- **Data Logging**: Comprehensive logging for development and testing

### Usage Examples

#### Injector Coding Procedure
1. Select "ECU Programming" → "Injector Coding"
2. Connect to vehicle ECU in extended diagnostic session
3. Read current injector codes from ECU
4. Input new injector flow rate codes
5. Program codes to ECU and verify operation

#### Real-time Chart Setup
1. Select "Realtime Charts"
2. Choose from predefined layouts or create custom
3. Configure chart types (gauge, line, bar, digital)
4. Set update rates and alert thresholds
5. Start recording for data analysis

#### Battery Registration
1. Select "Advanced Diagnostics" → "Battery Registration"
2. Enter new battery specifications (capacity, CCA, type)
3. Update Battery Management System parameters
4. Reset battery age and cycle counters

### Building the Enhanced Version

#### Prerequisites
- MinGW compiler with C99 support
- Allegro 4.2.3 graphics library
- Enhanced makefile includes new object files

#### Compilation
```bash
# Update makefile includes enhanced modules
make clean
make

# For debug build
make DEBUGMODE=1

# For static linking
make STATICLINK=1
```

#### Installation
1. Copy enhanced ScanTool.exe to desired location
2. Include all required DLL files from Allegro
3. Copy data files (scantool.dat, codes.dat)
4. Configure hardware interface settings

### Hardware Requirements

#### Minimum Requirements
- ELM327 or compatible OBD-II interface
- Windows XP or later
- 512MB RAM
- 50MB disk space

#### Recommended for Enhanced Features
- High-speed CAN interface (500kbps+)
- Windows 10 or later  
- 2GB RAM for real-time charts
- 1GB disk space for data recording

### License and Warranty

This enhanced version maintains the original open-source license. Enhanced features are provided as-is for educational and development purposes. Always follow proper safety procedures when working on vehicles.

### Support and Development

For issues specific to enhanced features:
1. Check diagnostic logs in application directory
2. Verify vehicle compatibility and prerequisites
3. Ensure proper hardware interface configuration
4. Review communication protocol settings

### Future Enhancements

Planned future additions:
- **CAN Bus Analyzer**: Real-time CAN message analysis
- **Script Engine**: Custom diagnostic script execution
- **Plugin Architecture**: Support for manufacturer-specific modules
- **Cloud Integration**: Remote diagnostic and firmware updates
- **Mobile Interface**: Smartphone/tablet companion app