# simplify
Simplify a list of files and folders.

Given a sequence of input paths on stdin, produces the shortest possible form of that list on stdout.
That is, given as input:
```
/bin
/bin/bash
/bin/grep
/bin/find
```
Output shall be only:
```
/bin
```
Exclusions can be specified such that a directory is not used if it contains a path to be excluded.
Excluding `/bin/grep` will produce:
```
/bin/bash
/bin/find
```

## Installation
### Requirements
* Boost (system, file system, program options)
* Boost Build

Download and extract the source or clone this repository.
```
b2 variant=release install
```
Installs into the default location (`/usr/local`) which can be overridden with `--prefix=/usr`.

## Usage
```
~ $ simplify --help
Simplify options:
  -h [ --help ]           Show this help message
  -x [ --exclude ] arg    Add a directory to exclusion list
  -X [ --excludemounts ]  Add all mount points to exclusion list

```
```
~ $ simplifind --help
Simplifind options:
  -h [ --help ]           Show this help message
  -x [ --exclude ] arg    Add a directory to exclusion list
  -X [ --excludemounts ]  Add all mount points to exclusion list
  -r [ --root ] arg       Add a root to search

```
