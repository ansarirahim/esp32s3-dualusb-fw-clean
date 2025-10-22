/**
 * @file test_filesystem.c
 * @brief Unit Tests for Filesystem Module
 *
 * @author A.R. Ansari <ansarirahim1@gmail.com>
 * @date 2025-10-21
 * @version 1.0.0
 *
 * @section description Description
 * Comprehensive unit tests for filesystem functionality.
 * Tests filesystem initialization, file operations, and statistics.
 *
 * @section test_cases Test Cases
 * - Filesystem initialization
 * - File existence checking
 * - Test file writing
 * - Filesystem statistics
 * - Filesystem mount/unmount
 * - Filesystem remount
 *
 * @section contact Contact
 * - Email: ansarirahim1@gmail.com
 * - GitHub: https://github.com/ansarirahim
 *
 * @section license License
 * Copyright (c) 2025 A.R. Ansari. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "unity.h"
#include "filesystem.h"

/**
 * @brief Setup function called before each test
 *
 * Initializes the filesystem for testing.
 */
void setUp(void) {
    /* Initialize filesystem for each test */
    fs_init_internal();
}

/**
 * @brief Teardown function called after each test
 *
 * Cleans up after each test.
 */
void tearDown(void) {
    /* Unmount filesystem after each test */
    fs_unmount();
}

/**
 * @test Filesystem Initialization
 *
 * Verifies that filesystem initializes successfully.
 */
TEST_CASE("FS: Initialization", "[filesystem]") {
    bool result = fs_init_internal();
    TEST_ASSERT_TRUE(result);
}

/**
 * @test Filesystem README File Creation
 *
 * Verifies that README.txt is created on first initialization.
 */
TEST_CASE("FS: README File Creation", "[filesystem]") {
    bool exists = fs_exists("/storage/README.txt");
    TEST_ASSERT_TRUE(exists);
}

/**
 * @test File Existence Check - Existing File
 *
 * Verifies that fs_exists returns true for existing files.
 */
TEST_CASE("FS: File Existence - Existing File", "[filesystem]") {
    /* README.txt should exist after initialization */
    bool exists = fs_exists("/storage/README.txt");
    TEST_ASSERT_TRUE(exists);
}

/**
 * @test File Existence Check - Non-existing File
 *
 * Verifies that fs_exists returns false for non-existing files.
 */
TEST_CASE("FS: File Existence - Non-existing File", "[filesystem]") {
    bool exists = fs_exists("/storage/nonexistent_file.txt");
    TEST_ASSERT_FALSE(exists);
}

/**
 * @test File Existence Check - NULL Path
 *
 * Verifies that fs_exists handles NULL path gracefully.
 */
TEST_CASE("FS: File Existence - NULL Path", "[filesystem]") {
    bool exists = fs_exists(NULL);
    TEST_ASSERT_FALSE(exists);
}

/**
 * @test Test File Writing
 *
 * Verifies that test file can be written successfully.
 */
TEST_CASE("FS: Test File Writing", "[filesystem]") {
    bool result = fs_write_test_file();
    TEST_ASSERT_TRUE(result);
}

/**
 * @test Test File Existence After Writing
 *
 * Verifies that test file exists after writing.
 */
TEST_CASE("FS: Test File Existence After Writing", "[filesystem]") {
    fs_write_test_file();
    bool exists = fs_exists("/storage/test_write.txt");
    TEST_ASSERT_TRUE(exists);
}

/**
 * @test Filesystem Statistics - Valid Pointers
 *
 * Verifies that filesystem statistics can be retrieved.
 */
TEST_CASE("FS: Statistics - Valid Pointers", "[filesystem]") {
    uint64_t total = 0, free = 0;
    bool result = fs_get_stats(&total, &free);
    
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_GREATER_THAN(0, total);
    TEST_ASSERT_GREATER_THAN_OR_EQUAL(0, free);
    TEST_ASSERT_LESS_THAN_OR_EQUAL(free, total);
}

/**
 * @test Filesystem Statistics - NULL Total Pointer
 *
 * Verifies that fs_get_stats handles NULL total pointer.
 */
TEST_CASE("FS: Statistics - NULL Total Pointer", "[filesystem]") {
    uint64_t free = 0;
    bool result = fs_get_stats(NULL, &free);
    TEST_ASSERT_FALSE(result);
}

/**
 * @test Filesystem Statistics - NULL Free Pointer
 *
 * Verifies that fs_get_stats handles NULL free pointer.
 */
TEST_CASE("FS: Statistics - NULL Free Pointer", "[filesystem]") {
    uint64_t total = 0;
    bool result = fs_get_stats(&total, NULL);
    TEST_ASSERT_FALSE(result);
}

/**
 * @test Filesystem Statistics - Both NULL Pointers
 *
 * Verifies that fs_get_stats handles both NULL pointers.
 */
TEST_CASE("FS: Statistics - Both NULL Pointers", "[filesystem]") {
    bool result = fs_get_stats(NULL, NULL);
    TEST_ASSERT_FALSE(result);
}

/**
 * @test Filesystem Unmount
 *
 * Verifies that filesystem can be unmounted.
 */
TEST_CASE("FS: Unmount", "[filesystem]") {
    bool result = fs_unmount();
    TEST_ASSERT_TRUE(result);
}

/**
 * @test Filesystem Remount
 *
 * Verifies that filesystem can be remounted after unmounting.
 */
TEST_CASE("FS: Remount", "[filesystem]") {
    fs_unmount();
    bool result = fs_remount();
    TEST_ASSERT_TRUE(result);
}

/**
 * @test Filesystem Remount - File Persistence
 *
 * Verifies that files persist after remount.
 */
TEST_CASE("FS: Remount - File Persistence", "[filesystem]") {
    /* Write test file */
    fs_write_test_file();
    
    /* Unmount and remount */
    fs_unmount();
    fs_remount();
    
    /* File should still exist */
    bool exists = fs_exists("/storage/test_write.txt");
    TEST_ASSERT_TRUE(exists);
}

/**
 * @test Filesystem Statistics Consistency
 *
 * Verifies that filesystem statistics are consistent.
 */
TEST_CASE("FS: Statistics Consistency", "[filesystem]") {
    uint64_t total1 = 0, free1 = 0;
    uint64_t total2 = 0, free2 = 0;
    
    fs_get_stats(&total1, &free1);
    fs_get_stats(&total2, &free2);
    
    /* Statistics should be consistent */
    TEST_ASSERT_EQUAL(total1, total2);
    TEST_ASSERT_EQUAL(free1, free2);
}

/**
 * @test Multiple File Operations
 *
 * Verifies that multiple file operations work correctly.
 */
TEST_CASE("FS: Multiple File Operations", "[filesystem]") {
    for (int i = 0; i < 5; i++) {
        bool result = fs_write_test_file();
        TEST_ASSERT_TRUE(result);
    }
    
    bool exists = fs_exists("/storage/test_write.txt");
    TEST_ASSERT_TRUE(exists);
}

