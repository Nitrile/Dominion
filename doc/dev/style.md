# Style guide

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
