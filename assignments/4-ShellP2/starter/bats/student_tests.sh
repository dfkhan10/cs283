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

@test "Echo outputs correct text" {
    run ./dsh <<EOF
echo "Hello, world!"
EOF

    # Check if the output is correct
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Hello, world!" ]
}

@test "Change directory works correctly" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF

    # Check if the pwd command returns /tmp
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" =~ /tmp ]]
}

@test "Handle command not found" {
    run ./dsh <<EOF
command_that_does_not_exist
EOF
    [ "${lines[0]}" = "CMD_ERR_EXECUTE" ]
}

@test "ls with arguments" {
    run ./dsh <<EOF
ls -l /tmp
EOF

    # Check success status
    [ "$status" -eq 0 ]
    # Optionally check for specific output characteristics
    # [ "${#lines[@]}" -gt 1 ] # Check if there is more than one line of output
}

@test "Execute a simple script" {
    run ./dsh <<EOF
echo "Start script"
echo "End script"
EOF

    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Start script" ]
    [ "${lines[1]}" = "End script" ]
}