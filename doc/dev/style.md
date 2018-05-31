# Style guide

## Format

File name extension conventions:
* `.hpp,.cpp`: C++
* `.hs`: Haskell
* `.py`: Python
* `.sh`: Shell script

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

## Naming

When naming:

1. All classes must be in CamelCase.
2. If a `.hpp` or `.cpp` file contains a class and the class is the main
	purpose of that file, the file name should match the class name exactly.
3. Fields must have trailing underscores
4. Libraries must live in their own namespaces. The name of the namespace
	 should be (could be truncated) the library name. e.g. The library `Net`
	 lives in `namespace Net { ... }`.
5. Libraries should have concise names.

## Branch

In addition to the following branches, each library and executable (i.e.
target) should have its own branch and maintains its own documentation and
tests:

* `master`: Main branch
* `build`: Build system

If you are working on an old branch, fast forward from `master` to avoid
conflicts.
