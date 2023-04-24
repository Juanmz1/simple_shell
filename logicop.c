#!/bin/bash

/**
Declare a global associative array called "aliases"
declare -gA aliases
*/

/**
 * Prints the aliases. If no arguments are provided, prints all aliases.
 * If one or more arguments are provided, prints the aliases for those keys.
 *
 * @param: args The keys to print aliases for (optional)
 */
void print_aliases()
{
/**
If no arguments are provided, print all aliases
*/
if test $# -eq 0; then
for key in "${!aliases[@]}"; do
echo "$key='${aliases[$key]}'"
done
/**
If one or more arguments are provided, print the aliases for those keys
*/
else
for key in "$@"; do
echo "$key='${aliases[$key]}'"
done
fi
}

/**
 * Sets the aliases. Loops through each argument and sets an alias for each one
 * that contains an equal sign.
 *
 * @param args The aliases to set
 */
function set_aliases
{
/**
Loop through each argument
*/
for arg in "$@"; do
/**
If the argument contains an equal sign, set the alias
*/
if [[ "$arg" == *=* ]]; then
key="${arg%=*}"
value="${arg#*=}"
aliases["$key"]="$value"
/**
If the argument does not contain an equal sign, print an error message
*/
else
echo "Error: invalid argument '$arg'"
fi
done
}

/**
 * Handles the && and || operators. Loops through the arguments and executes
 * commands based on the logical operator that was previously set.
 *
 * @param args The commands to execute
 */
function handle_logical
{
/**
Loop through the arguments
*/
for arg in "$@"; do
/**
If the argument is "&&" or "||", set the logical operator
*/
if [[ "$arg" == "&&" || "$arg" == "||" ]]; then
logical_op="$arg"
/**
If the argument is not a logical operator, execute the command and handle the result
*/
else
/**
If the logical operator is "&&" and the previous command succeeded, execute the current command
*/
if [[ "$logical_op" == "&&" && "$?" -eq 0 ]]; then
eval "$arg"
/**
If the logical operator is "||" and the previous command failed, execute the current command
*/
elif [[ "$logical_op" == "||" && "$?" -ne 0 ]]; then
eval "$arg"
fi
/**
Reset the logical operator
*/
logical_op=""
fi
done
}

/**
If no arguments are provided, print all aliases
*/
if [[ $# -eq 0 ]]; then
print_aliases
/**
If the first argument is "alias", set or print aliases as appropriate
*/
elif [[ "$1" == "alias" ]]; then
shift
if [[ $# -eq 0 ]]; then
print_aliases
else
set_aliases "$@"
fi
/**
If the first argument contains "&&" or "||", handle the logical operators and execute the commands
*/
elif [[ "$*" == *"&&"* || "$*" == *"||"* ]]; then
handle_logical "$@"
/**
If the first argument is not "alias" or a logical operator, print an error message
*/
else
echo "Error: unknown command '$1'"
fi
