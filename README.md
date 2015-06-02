# rand

This program generates random numbers in a user-specified range using the random devices `/dev/random` and `/dev/urandom`.

## Usage

Get a usage message:

    rand -h

Generate a random number in the range from 1 to 10 (inclusive):

    rand 1 10

Generate a random number from 0 to `UINT_MAX`:

    rand

Generate ten random numbers using `/dev/random` rather than `/dev/urandom`:

    rand -n 10 -r

