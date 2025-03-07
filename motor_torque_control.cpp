/*
 * IMPORTANT: libmodbus is a C library, so we have to be careful and use c-style notation while using C++
 * i.e. use C-style string literals instead of std::string
 * 
 * Q: Why use libmodbus over a C++ library?
 * A: 
 *    - Most commonly used library for modbus RTU in C/C++.
 *    - Amazing Reference guide 
*/

#include <iostream>
#include <modbus/modbus.h>
#include <chrono>
#include <thread>

void write_register(modbus_t *ctx, uint16_t reg_addr, uint16_t value, const std::string &desc) {
    if (modbus_write_register(ctx, reg_addr, value) == -1) {
        std::cerr << "Failed to set " << desc << ": " << modbus_strerror(errno) << "\n";
    } else {
        std::cout << desc << " set successfully\n";
    }
}

int main() {

    // Modbus RTU info
    const char*     DEVICE              = "/dev/ttyUSB0";  // Serial Port
    const int       BAUD_RATE           = 115200;
    const int       DEVICE_ID           = 1;  // Motor Controller Address

    // Motor Register Addresses
    const uint16_t  REG_ADDR_OP_MODE    = 0x6600; 
    const uint16_t  REG_ADDR_MAX_TORQUE = 0x6720; 
    const uint16_t  REG_ADDR_TRQ_SLOPE  = 0x6870; 
    const uint16_t  REG_ADDR_TGT_TORQUE = 0x6710; 
    const uint16_t  REG_ADDR_CNTRL_WORD = 0x6400; 

    // Create a Modbus RTU context
    modbus_t *ctx = modbus_new_rtu(DEVICE, BAUD_RATE, 'N', 8, 1);
    if (ctx == nullptr) {
        std::cerr << "Unable to create the libmodbus context\n";
        return -1;
    }

    // Set motor controller as slave device
    if (modbus_set_slave(ctx, DEVICE_ID) == -1) {
        std::cerr << "Invalid slave ID\n";
        modbus_free(ctx);
        return -1;
    }

    // Open the RS-485 serial connection
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return -1;
    }

    std::cout << "Connected to motor controller ID: " << DEVICE_ID << " at " << BAUD_RATE << " baud\n";

    // 1. Set mode to Profile Torque Mode (PT Mode)
    write_register(ctx, REG_ADDR_OP_MODE, 0x04, "Profile Torque Mode");

    // 2. Set the max torque to 3000‰
    write_register(ctx, REG_ADDR_MAX_TORQUE, 0x0BB8, "Max Torque (3000‰)");

    // 3. Set the torque slope to 1000‰/s
    write_register(ctx, REG_ADDR_TRQ_SLOPE, 0x03E8, "Torque Slope (1000‰/s)");

    // 4. Set the target torque to 10‰
    write_register(ctx, REG_ADDR_TGT_TORQUE, 0xA, "Target Torque (10‰)");

    // 5. Shutdown command
    write_register(ctx, REG_ADDR_CNTRL_WORD, 0x0006, "Motor Shutdown");

    // 6. Switch on and enable operation command
    write_register(ctx, REG_ADDR_CNTRL_WORD, 0x000F, "Enable Motor Operation");

    // 7. Run for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 8. Stop torque execution
    write_register(ctx, REG_ADDR_CNTRL_WORD, 0x0006, "Motor Shutdown");

    // 9. Close connection
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}

