#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# ✅ Test: Execute simple commands
@test "Execute ls command" {
    run ./dsh <<< "ls"
    [[ "$output" =~ "dsh" ]]  # Adjust based on actual output
}

@test "Execute echo command" {
    run ./dsh <<< "echo Hello, World"
    [[ "$output" =~ "Hello, World" ]]
}

# ✅ Test: Handle quoted spaces
@test "Handle quoted spaces in echo" {
    run ./dsh <<< 'echo "  hello   world  "'
    [[ "$output" =~ "  hello   world  " ]]
}

# ✅ Test: Piped Commands
@test "Pipe: ls | grep .c" {
    run ./dsh <<< "ls | grep .c"
    [[ "$output" =~ ".c" ]]
}

@test "Pipe: echo test | tr a-z A-Z" {
    run ./dsh <<< "echo test | tr a-z A-Z"

    # Debugging output
    echo "==== DEBUG: ACTUAL OUTPUT ===="
    echo "$output"
    echo "============================="

    # Check if output contains "TEST"
    [[ "$output" =~ "TEST" ]]
}

@test "Pipe: multiple pipes" {
    run ./dsh <<< "echo Hello | tr a-z A-Z | rev"

    # Remove prompts and extra messages
    cleaned_output=$(echo "$output" | sed 's/dsh3>//g' | grep -v "cmd loop returned" | tr -d '[:space:]')

    [[ "$cleaned_output" == "OLLEH" ]]
}

# ✅ Test: Built-in 'cd' command
@test "Built-in: cd to /tmp" {
    run ./dsh <<< "cd /tmp && pwd"
    [[ "$output" =~ "/tmp" ]]
}

@test "Built-in: cd without argument" {
    run ./dsh <<< "cd"
    [[ "$status" -eq 0 ]]  # Should not fail, but may not change dir
}

# ✅ Test: Empty Input
@test "Handle empty input" {
    run ./dsh <<EOF

EOF
    [[ "$status" -eq 0 ]]
}

# ✅ Test: Too many pipes (limit should be 8)
@test "Too many pipes error" {
    run ./dsh <<< "ls | grep .c | wc -l | cat | cat | cat | cat | cat | cat | cat"
    [[ "$output" =~ "CMD_ERR_PIPE_LIMIT" ]]
}

# ✅ Test: Invalid command handling
@test "Invalid command execution" {
    run ./dsh <<< "nonexistentcommand"
    [[ "$output" =~ "CMD_ERR_EXECUTE" ]]
}

# ✅ Test: Exit command
@test "Exit command" {
    run ./dsh <<< "exit"
    [[ "$status" -eq 0 ]]
}

# ✅ Test: Max allowed piped commands (should pass)
@test "Max allowed (8) piped commands" {
    run ./dsh <<< "cmd1 | cmd2 | cmd3 | cmd4 | cmd5 | cmd6 | cmd7 | cmd8"
    [[ "$status" -eq 0 ]]
}

# ✅ Test: More than 8 commands should fail
@test "Too many commands (>8) should fail" {
    run ./dsh <<< "cmd1 | cmd2 | cmd3 | cmd4 | cmd5 | cmd6 | cmd7 | cmd8 | cmd9"
    [[ "$output" =~ "CMD_ERR_PIPE_LIMIT" ]]
}