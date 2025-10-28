# Permissions

## Users & Groups

```/etc/passwd``` 
username:password:userId:groupID:comment:homeDir:bash

```/etc/shadow```
username:password:lastChanged:min:max:warn:inactive:expire:futureUse

```useradd -u userID -g groupID -c "Comment" -md /home/user -s fullPathOfShell userName```

```passwd userName```

```groupadd -g groupID groupName```

```usermod -g primaryGroup -aG secondaryGroup userName```

```ls -la /etc/skel``` -> config files by default created for new users

```useradd -D```

```newusers filename```

```usermod -l newLoginName -L lockPassword -U unlockPassword```

```userdel -r username```

Change group of the files ```chgrp groupName fileNames``` -> ```chgrp os f1 f2 f3```

Temporarily change the primary group of user ```newgrp os``` -> ```exit```

Switch to another user with known commands ```su - user -c command options arguments; ...```

chown user1 file1

chown user1:group1 file1

chown :group1 file1

---

ls -l
Owner, Group, Others #ofLinks ownerName groupName sizeOfFile dateOfCreation nameOfFile

## Permissions

|Permission|File Access                     |Directory Access               |
|----------|--------------------------------|-------------------------------|
|Read      |Display file contents & copy    |Can list the directory content |
|Write     |Modify the file contents        |wx -> add and delete files     |
|Execute   |Execute/if shell and rx         |cd, rx -> ls -l                |

```chmod permission filename```

Permissions specified in either:
- Symbolic mode:
    - Who
        - u: owner permissions
        - g: group permissions
        - o: other permissions
        - a: all   permissions
    - Operator
        - +: add    permissions
        - -: remove permissions
        - =: assign permissions
    - Permissions
        - r: read    permission
        - w: write   permission
        - x: execute permission
- Octal mode:
    - 4: read
    - 2: write
    - 1: execute

example: ```chmod u+x,go+r file1```, ```chmod a=rw file1```, ```chmod 555 file1```
