# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run_tests.sh                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/20 21:46:37 by krahnama          #+#    #+#              #
#    Updated: 2026/07/20 21:46:57 by krahnama         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}╔══════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║   GET_NEXT_LINE AUTO-TEST SUITE        ║${NC}"
echo -e "${BLUE}╚══════════════════════════════════════════╝${NC}"

# Test different BUFFER_SIZE values
BUFFER_SIZES=(1 2 4 8 16 32 42 64 100 1024 4096 9999)

for SIZE in "${BUFFER_SIZES[@]}"; do
    echo -e "\n${YELLOW}Testing with BUFFER_SIZE = $SIZE${NC}"
    echo "----------------------------------------"
    
    # Compile
    gcc -Wall -Wextra -Werror -D BUFFER_SIZE=$SIZE \
        test_gnl.c get_next_line.c get_next_line_utils.c -o test_gnl 2> compile_errors.txt
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Compilation failed!${NC}"
        cat compile_errors.txt
        exit 1
    fi
    
    # Run tests
    ./test_gnl
    
    # Check for memory leaks with valgrind if available
    if command -v valgrind &> /dev/null; then
        echo -e "\n${BLUE}Running valgrind memory check...${NC}"
        valgrind --leak-check=full \
                 --show-leak-kinds=all \
                 --track-origins=yes \
                 --verbose \
                 --log-file=valgrind_output.txt \
                 ./test_gnl > /dev/null 2>&1
        
        if grep -q "All heap blocks were freed -- no leaks are possible" valgrind_output.txt; then
            echo -e "${GREEN}✓ No memory leaks detected${NC}"
        else
            echo -e "${RED}✗ Memory leaks detected!${NC}"
            grep -A 10 "LEAK SUMMARY" valgrind_output.txt
        fi
    else
        echo -e "${YELLOW}⚠️  Valgrind not installed - skipping memory check${NC}"
    fi
    
    rm -f test_gnl valgrind_output.txt
done

echo -e "\n${GREEN}All tests completed!${NC}"