# Test Cases
These are the formats that should be parseable.

TODO: Add automated tests.

## Valid:
For:
 - Whole Number Separator: %
 - Fraction Separator: -

### Positive
A B-Z   1 2-3   => 5/3  PASS
A-B-Z   1-2-3   => 5/3  PASS
A%B-Z   1%2-3   => 5/3  PASS
B/Z     5/3     => 5/3  PASS
B-Z     2-3     => 2/3  PASS
Z       3       => 3    PASS

### Negative
-A B-Z   -1 2-3 => -5/3 PASS
-A-B-Z   -1-2-3 => -5/3 PASS
-A%B-Z   -1%2-3 => -5/3 PASS
-B/Z     -5/3   => -5/3 PASS
-B-Z     -2-3   => -2/3 PASS
-Z       -3     => -3   PASS

## Invalid:
A?B-Z   -1?2-3          PASS
A-B?Z   -1-2?3          PASS
A%B?Z   1%2?3           PASS
B Z     2 3             PASS
        FAIL            PASS