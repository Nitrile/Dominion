# Style guide

## Format

The source code is formatted according to the following astyle
configuration:
```
--style=allman
--align-pointer=type	
--close-templates
--convert-tabs
--indent-preproc-block
--indent-preproc-define
--indent=tab=2
--pad-header
--unpad-paren
```
When naming:

1. All classes must be in CamelCase
2. Each module resides in its own namespace
3. Fields must have trailing underscores

## Branch

In addition to the following branches, each library and executable (i.e.
target) should have its own branch:

* `master`: Main branch
* `build`: Build system

Library and executable branches:

* `net`: `$SL/Net`
* `client`: `$SC`
* `server`: `$SS`
