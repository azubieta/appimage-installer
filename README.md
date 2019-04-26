# appimage-cli-tool

CLI client for <http://www.appimagehub.com>. Allows to search, install, remove and update applications using the AppImage 
package format.

## Usage
```bash
> app --help
Usage: app [options] command
Command details
   search <query>              list applications available in the store
   install <STORE ID>          install the application with the given store id
   list                        list applications available on your system
   update <APP ID>             update if possible the given application
   remove <APP ID>             remove the application from your system

Options:
  -h, --help  Displays this help.

Arguments:
  command     Command to be executed: search | install | list | update | remove

> app search firefox
1168996 Firefox - Appimage 63.0.1 by AJSlye

> app install 1168996
Getting 1168996 from https://dl.opendesktop.org/api/files/download/id/1530021194/s/0cb9c26e57f7011261ca000b778cf021/t/1553641278/o/1/Firefox-60.0.glibc2.7-x86_64.AppImage

```
