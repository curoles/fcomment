# fcomment - Attach a comment string to a file

**fcomment** is a CLI tool to:
- attach a comment string to a file;
- show attached comment;
- list files in a directory showing attached comment.

## What is _comment_?

_Comment_ is short text associated with a file.

## Why do I need this tool?

Purpose of **fcomment** is to document files and directories.
Attached to a file _comment_ provides short explanation of the file.

Usage cases:
- To document multiple source code files in a big project
  to give others brief overview of what is inside each file (without opening files).
- To document installed by your installer binary executable and library files.
- To document temporary and log files created by your application
  for post-analysis.

## Do not Linux commands `attr`, `getfattr` and `setfattr` from attr package already do it?

When possible **fcomment** uses **Extended File Attributes** as secondary
mechanism to store _comment_.
Primary saving mechanism uses hidden files.

Primary saving mechanism saves file meta data when Extended File Attributes may fail to do the job:
- File system does not support **Extended File Attributes**.
- User has no access to the tools to modify Extended File Attributes.
- File is NFS mounted (need to double check).
- File is stored somewhere in a cloud. For example, source code on Github.

## Can I see _comment_ with `attr` command?

Yes, if **Extended File Attributes** are supported.
**fcomment** uses attribute name "comment" in "user" category, full name is "user.comment".

## Single executable

**fcomment** is implemented as single tool-box program.
To select a tool use first command line argument or create a symbolic link in form `fcomment-<cmd>`.

| Function    | File name    | Command       | Arguments               |
| ----------- | ------------ | ------------- | ----------------------- |
| Get comment | fcomment-ls  | fcomment [ls] | PATH \[OPTIONS\]        |
| Set comment | fcomment-set | fcomment set  | PATH COMMENT \[OPTIONS\]|
| Copy comment| fcomment-cp  | fcomment cp   | SRC DEST \[OPTIONS\]    |

## Set comment

An example from **fcomment** Makefile where freshly compiled executable
called `fcomment` is used to attach a comment to itself.

```
./fcomment set ./fcomment "CLI program to 1) attach comment to a file and 2) view attached comment"
```

## Get comment

An example of how to get _comment_ text associated with file `fcomment`. 
```Text
igor:~/prj/fcomment/build$ ./fcomment ./fcomment 
CLI program to 1) attach comment to a file and 2) view attached comment
```

An example of how to get _comment_ with `attr`.
```Text
igor:~/prj/fcomment/build$ attr -g comment ./fcomment 
Attribute "comment" had a 72 byte value for ./fcomment:
CLI program to 1) attach comment to a file and 2) view attached comment
```

## List files

When PATH is a directory, `fcomment` lists all files inside the directory
(including itself) and shows the comments.

```Text
igor:~/prj/fcomment/build$ ./fcomment 
. - fcomment project build directory with compiled program and temporary files
..
Makefile
config.makefile
fcomment - CLI program to 1) attach comment to a file and 2) view attached comment
```

## Copy and move

## Synchronization

