# rand

This program generates random numbers in a user-specified range using the random device `/dev/random` (or alternately `/dev/urandom`).

## Usage

Get a usage message:

    rand -h

Generate a random number in the range from 1 to 10 (inclusive):

    rand 1 10

Generate a random number from 0 to `INT_MAX`:

    rand

Generate ten random numbers using `/dev/urandom` rather than `/dev/random`:

    rand -n 10 -u

