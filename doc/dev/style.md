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
Always use tabs when changing the tab size does not destroy alignment.
Otherwise, use spaces. This rule applies to all non-python code, including
Haskell. e.g.
```
class Cl
{
	void func(int a,
	          int b,
	          int c,
	         );
};
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

* `release`: Release branch
* `master`: Main branch. Active development happens here but the head build
	must always be buildable and executable.
* `build`: Build system

If you are working on an old branch, merge with `--no-ff` from `master` to
avoid later conflicts.

Always use `--no-ff` when merging into development and release branches!
