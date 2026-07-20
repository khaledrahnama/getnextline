/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_gnl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krahnama <krahnama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 21:45:28 by krahnama          #+#    #+#             */
/*   Updated: 2026/07/20 21:54:52 by krahnama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Colors for better output
#define GREEN   "\033[0;32m"
#define RED     "\033[0;31m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

int tests_passed = 0;
int tests_failed = 0;

void print_test_result(const char *test_name, int passed)
{
    if (passed)
    {
        printf(GREEN "✓ PASS: %s\n" RESET, test_name);
        tests_passed++;
    }
    else
    {
        printf(RED "✗ FAIL: %s\n" RESET, test_name);
        tests_failed++;
    }
}

void create_test_file(const char *filename, const char *content)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Failed to create test file");
        return;
    }
    write(fd, content, strlen(content));
    close(fd);
}

// ==================== TEST 1: Basic Single Line ====================
void test_basic_single_line(void)
{
    printf(BLUE "\n=== TEST 1: Basic Single Line ===\n" RESET);
    create_test_file("test1.txt", "Hello World!\n");
    
    int fd = open("test1.txt", O_RDONLY);
    char *line = get_next_line(fd);
    
    int passed = (line && strcmp(line, "Hello World!\n") == 0);
    print_test_result("Single line with newline", passed);
    free(line);
    close(fd);
}

// ==================== TEST 2: Multiple Lines ====================
void test_multiple_lines(void)
{
    printf(BLUE "\n=== TEST 2: Multiple Lines ===\n" RESET);
    create_test_file("test2.txt", "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n");
    
    int fd = open("test2.txt", O_RDONLY);
    char *expected[] = {"Line 1\n", "Line 2\n", "Line 3\n", "Line 4\n", "Line 5\n"};
    int passed = 1;
    
    for (int i = 0; i < 5; i++)
    {
        char *line = get_next_line(fd);
        if (!line || strcmp(line, expected[i]) != 0)
        {
            passed = 0;
            free(line);
            break;
        }
        free(line);
    }
    
    print_test_result("Multiple lines (5 lines)", passed);
    close(fd);
}

// ==================== TEST 3: File Without Newline ====================
void test_no_newline(void)
{
    printf(BLUE "\n=== TEST 3: File Without Trailing Newline ===\n" RESET);
    create_test_file("test3.txt", "Line 1\nLine 2\nLine 3");
    
    int fd = open("test3.txt", O_RDONLY);
    char *line1 = get_next_line(fd);
    char *line2 = get_next_line(fd);
    char *line3 = get_next_line(fd);
    
    int passed = (line1 && strcmp(line1, "Line 1\n") == 0 &&
                  line2 && strcmp(line2, "Line 2\n") == 0 &&
                  line3 && strcmp(line3, "Line 3") == 0);
    
    print_test_result("No trailing newline", passed);
    free(line1);
    free(line2);
    free(line3);
    close(fd);
}

// ==================== TEST 4: Empty File ====================
void test_empty_file(void)
{
    printf(BLUE "\n=== TEST 4: Empty File ===\n" RESET);
    create_test_file("test4.txt", "");
    
    int fd = open("test4.txt", O_RDONLY);
    char *line = get_next_line(fd);
    
    int passed = (line == NULL);
    print_test_result("Empty file returns NULL", passed);
    free(line);
    close(fd);
}

// ==================== TEST 5: File With Only Newline ====================
void test_only_newline(void)
{
    printf(BLUE "\n=== TEST 5: File With Only Newline ===\n" RESET);
    create_test_file("test5.txt", "\n");
    
    int fd = open("test5.txt", O_RDONLY);
    char *line = get_next_line(fd);
    
    int passed = (line && strcmp(line, "\n") == 0);
    print_test_result("Only newline", passed);
    free(line);
    close(fd);
}

// ==================== TEST 6: Very Long Line ====================
void test_long_line(void)
{
    printf(BLUE "\n=== TEST 6: Very Long Line ===\n" RESET);
    char *long_line = malloc(10002);
    if (!long_line)
        return;
    
    for (int i = 0; i < 10000; i++)
        long_line[i] = 'A';
    long_line[10000] = '\n';
    long_line[10001] = '\0';
    
    create_test_file("test6.txt", long_line);
    
    int fd = open("test6.txt", O_RDONLY);
    char *line = get_next_line(fd);
    
    int passed = (line && (int)strlen(line) == 10001);
    print_test_result("Long line (10000 chars)", passed);
    free(line);
    free(long_line);
    close(fd);
}

// ==================== TEST 7: Multiple FDs ====================
void test_multiple_fds(void)
{
    printf(BLUE "\n=== TEST 7: Multiple File Descriptors ===\n" RESET);
    create_test_file("test7a.txt", "File A - Line 1\nFile A - Line 2\n");
    create_test_file("test7b.txt", "File B - Line 1\nFile B - Line 2\n");
    
    int fd1 = open("test7a.txt", O_RDONLY);
    int fd2 = open("test7b.txt", O_RDONLY);
    
    char *line1a = get_next_line(fd1);
    char *line2a = get_next_line(fd2);
    char *line3a = get_next_line(fd1);
    char *line4a = get_next_line(fd2);
    
    int passed = (line1a && strcmp(line1a, "File A - Line 1\n") == 0 &&
                  line2a && strcmp(line2a, "File B - Line 1\n") == 0 &&
                  line3a && strcmp(line3a, "File A - Line 2\n") == 0 &&
                  line4a && strcmp(line4a, "File B - Line 2\n") == 0);
    
    print_test_result("Multiple FDs interleaved", passed);
    free(line1a);
    free(line2a);
    free(line3a);
    free(line4a);
    close(fd1);
    close(fd2);
}

// ==================== TEST 8: Invalid FD ====================
void test_invalid_fd(void)
{
    printf(BLUE "\n=== TEST 8: Invalid File Descriptor ===\n" RESET);
    char *line = get_next_line(-1);
    
    int passed = (line == NULL);
    print_test_result("Invalid FD returns NULL", passed);
    free(line);
}

// ==================== TEST 9: Mixed Content ====================
void test_mixed_content(void)
{
    printf(BLUE "\n=== TEST 9: Mixed Content ===\n" RESET);
    create_test_file("test9.txt", "Hello\n\nWorld\n\n\n42\n");
    
    int fd = open("test9.txt", O_RDONLY);
    char *lines[6];
    int passed = 1;
    
    for (int i = 0; i < 6; i++)
    {
        lines[i] = get_next_line(fd);
        if (i == 1 && lines[i] && strcmp(lines[i], "\n") != 0)
            passed = 0;
        if (i == 3 && lines[i] && strcmp(lines[i], "\n") != 0)
            passed = 0;
        if (i == 4 && lines[i] && strcmp(lines[i], "\n") != 0)
            passed = 0;
    }
    
    print_test_result("Mixed content with empty lines", passed);
    for (int i = 0; i < 6; i++)
        free(lines[i]);
    close(fd);
}

// ==================== TEST 10: Standard Input ====================
void test_stdin(void)
{
    printf(BLUE "\n=== TEST 10: Standard Input ===\n" RESET);
    printf(YELLOW "This test requires manual input. Type 3 lines and press Ctrl+D:\n" RESET);
    
    char *line = get_next_line(0);
    int count = 0;
    while (line)
    {
        printf("Line %d: %s", ++count, line);
        free(line);
        line = get_next_line(0);
    }
    
    printf(GREEN "\n✓ PASS: stdin test completed (read %d lines)\n" RESET, count);
    tests_passed++;
}

// ==================== TEST 11: Large Buffer Size ====================
void test_large_buffer(void)
{
    printf(BLUE "\n=== TEST 11: Very Large Lines ===\n" RESET);
    int size = 500000;
    char *long_line = malloc(size + 2);
    if (!long_line)
        return;
    
    for (int i = 0; i < size; i++)
        long_line[i] = 'X';
    long_line[size] = '\n';
    long_line[size + 1] = '\0';
    
    create_test_file("test11.txt", long_line);
    
    int fd = open("test11.txt", O_RDONLY);
    char *line = get_next_line(fd);
    
    int passed = (line && (int)strlen(line) == size + 1);
    print_test_result("Very large line (500k chars)", passed);
    free(line);
    free(long_line);
    close(fd);
}

// ==================== TEST 12: Memory Leak Test ====================
void test_memory_leak(void)
{
    printf(BLUE "\n=== TEST 12: Memory Leak Test ===\n" RESET);
    printf(YELLOW "Run with valgrind to check for memory leaks\n" RESET);
    
    create_test_file("test12.txt", "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n");
    
    int fd = open("test12.txt", O_RDONLY);
    char *line;
    int count = 0;
    
    while ((line = get_next_line(fd)) != NULL)
    {
        free(line);
        count++;
    }
    
    print_test_result("No memory leaks (run valgrind to verify)", count == 5);
    close(fd);
}

// ==================== TEST 13: Special Characters ====================
void test_special_characters(void)
{
    printf(BLUE "\n=== TEST 13: Special Characters ===\n" RESET);
    create_test_file("test13.txt", "Tab\tseparated\nUTF-8: 你好\nSpecial: !@#$%^&*()\n");
    
    int fd = open("test13.txt", O_RDONLY);
    char *line1 = get_next_line(fd);
    char *line2 = get_next_line(fd);
    char *line3 = get_next_line(fd);
    
    int passed = (line1 && strstr(line1, "Tab\tseparated") &&
                  line2 && strstr(line2, "UTF-8: 你好") &&
                  line3 && strstr(line3, "Special: !@#$%^&*()"));
    
    print_test_result("Special characters and UTF-8", passed);
    free(line1);
    free(line2);
    free(line3);
    close(fd);
}

// ==================== TEST 14: Reading After EOF ====================
void test_after_eof(void)
{
    printf(BLUE "\n=== TEST 14: Reading After EOF ===\n" RESET);
    create_test_file("test14.txt", "Only one line\n");
    
    int fd = open("test14.txt", O_RDONLY);
    char *line1 = get_next_line(fd);
    char *line2 = get_next_line(fd);
    char *line3 = get_next_line(fd);
    
    int passed = (line1 && line2 == NULL && line3 == NULL);
    print_test_result("Returns NULL after EOF", passed);
    free(line1);
    close(fd);
}

// ==================== TEST 15: Binary Data ====================
void test_binary_data(void)
{
    printf(BLUE "\n=== TEST 15: Binary Data ===\n" RESET);
    unsigned char binary_data[] = {0x41, 0x42, 0x43, 0x0A, 0x44, 0x45, 0x46, 0x00, 0x47, 0x48, 0x49, 0x0A};
    
    int fd = open("test15.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, binary_data, sizeof(binary_data));
    close(fd);
    
    fd = open("test15.txt", O_RDONLY);
    char *line1 = get_next_line(fd);
    char *line2 = get_next_line(fd);
    
    int passed = (line1 && line2 && (int)strlen(line1) == 4 && (int)strlen(line2) >= 3);
    print_test_result("Binary data handling", passed);
    free(line1);
    free(line2);
    close(fd);
}

// ==================== CLEANUP ====================
void cleanup(void)
{
    const char *files[] = {
        "test1.txt", "test2.txt", "test3.txt", "test4.txt", "test5.txt",
        "test6.txt", "test7a.txt", "test7b.txt", "test9.txt", "test11.txt",
        "test12.txt", "test13.txt", "test14.txt", "test15.txt"
    };
    
    for (int i = 0; i < 14; i++)
        unlink(files[i]);
}

// ==================== MAIN ====================
int main(void)
{
    printf(GREEN "\n╔══════════════════════════════════════════╗\n" RESET);
    printf(GREEN "║   GET_NEXT_LINE TEST SUITE             ║\n" RESET);
    printf(GREEN "╚══════════════════════════════════════════╝\n" RESET);
    printf("BUFFER_SIZE = %d\n", BUFFER_SIZE);
    
    test_basic_single_line();
    test_multiple_lines();
    test_no_newline();
    test_empty_file();
    test_only_newline();
    test_long_line();
    test_multiple_fds();
    test_invalid_fd();
    test_mixed_content();
    test_large_buffer();
    test_memory_leak();
    test_special_characters();
    test_after_eof();
    test_binary_data();
    
    cleanup();
    
    printf(BLUE "\n╔══════════════════════════════════════════╗\n" RESET);
    printf(BLUE "║            TEST RESULTS                 ║\n" RESET);
    printf(BLUE "╚══════════════════════════════════════════╝\n" RESET);
    printf(GREEN "Passed: %d\n" RESET, tests_passed);
    printf(RED "Failed: %d\n" RESET, tests_failed);
    printf("Total:  %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0)
        printf(GREEN "\n🎉 ALL TESTS PASSED! 🎉\n\n" RESET);
    else
        printf(RED "\n⚠️  SOME TESTS FAILED!\n\n" RESET);
    
    return (tests_failed > 0);
}