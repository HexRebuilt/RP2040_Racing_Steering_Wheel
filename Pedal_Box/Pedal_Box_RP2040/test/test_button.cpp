#include <Arduino.h>
#include <cassert>
#include "../src/buttonhandler.h"
#include "../src/buttonIds.h"

int main() {
    // Test ButtonHandler convertIDtoPot
    printf("Testing convertIDtoPot...\n");
    
    // Test 1: Green button (id "1") should return POT_MIN (0)
    {
        ButtonHandler bh;
        short result = bh.convertIDtoPot(String("1"));
        printf("test_buttonhandler_green_button: ");
        assert(result == 0);
        printf("PASS\n");
    }
    
    // Test 2: Push button (id "9") should return POT_MAX (127)
    {
        ButtonHandler bh;
        short result = bh.convertIDtoPot(String("9"));
        printf("test_buttonhandler_push_button: ");
        assert(result == 127);
        printf("PASS\n");
    }
    
    // Test 3: Invalid button (id "99") should return 0
    {
        ButtonHandler bh;
        short result = bh.convertIDtoPot(String("99"));
        printf("test_buttonhandler_invalid_button: ");
        assert(result == 0);
        printf("PASS\n");
    }
    
    // Test 4: Button 5 should return value in range [0, 127]
    {
        ButtonHandler bh;
        short result = bh.convertIDtoPot(String("5"));
        printf("test_buttonhandler_button_5: ");
        assert(result >= 0 && result <= 127);
        printf("PASS\n");
    }
    
    printf("\n====================\n");
    printf("4 tests, 0 failures\n");
    printf("====================\n");
    
    return 0;
}