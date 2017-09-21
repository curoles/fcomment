# fcomment - Attach a comment string to a file

**fcomment** is a CLI tool to:
- attach a comment to a file;
- show attached comment;
- list files in a directory showing attached comment.

Purpose of **fcomment** is to document files and directories.
Attached to a file comment provides short explanation of the file.

Usage cases:
- Document multiple source code files in a big project
  to give others brief overview of what is inside each file.
- Document installed binary executable and library files.
- Document temporary and log files.


When possible **fcomment** uses extended file attributes as primary
mechanism to store _comment_.
Secondary mechanism uses hidden files; it allows to get _comment_ when
files are cloned from a source code repository.

## Set comment

## Get comment

## List files
