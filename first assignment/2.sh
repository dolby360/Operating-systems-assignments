#!/bin/bash
find -type f -name "*.sh" -exec chmod 750 {} \;
find -type f -name "*.*" ! -name "*.sh" -exec chmod u=rw,g=r,o-rwx {} \; 
find -type d -exec chmod u=rwx,g=x,o-rwx {} \;
