Intro to Git and Github
=======================


## Totally fixed this




You will not need a vagrant vm for this lab. The lab computers all have git installed.

Why Version Control?
--------------------

Version control is a way of recording changes to a set of files over time. You can use it to make sure new changes don't break the work you've already done. It is critical for collaboration on projects, since it keeps track of who made what changes when and provides means to resolve conflicts between two people's changes. In many version control systems, it allows for a central server where the versioned files are stored, which can serve as a remote backup mechanism.

Why Git?
--------

<!---
Largely based on http://thkoch2001.github.io/whygitisbetter
-->
- Git allows for local development, which means less network overhead
- Git is fast, partly because of the lack of network overhead and partly because of its snapshotting design
- Git is distributed: every client copy is a full backup of the server copy
- Git is versatile: there are innumerable ways to use git
- Git is the standard: modern companies are moving away from older version control technologies toward git

Initial Setup
-------------

- Tell git who you are. Why this is important will become clear later.

        $ git config --global user.name "Full Name"
        $ git config --global user.email "email@email.org"
        $

- Tell git what editor you use

        $ git config --global core.editor vim
        $

- It's possible you will like the default editor and you won't have to change this. Check with:

        $ git config core.editor
        vim
        $

- Tell Git that you want colors enabled!

        $ git config --global color.ui always
        $

- Take a look at your handywork:

        $ less ~/.gitconfig

- You can edit this file directly if you prefer.

Documentation
-------------

- You can look up some commands with `git --help`
- All git commands have a --help flag which brings up a man page
- You can also look up documentation for a git command with man directly: you just need to hyphenate the command, like

        $ man git-config

The Basics: a Local Repository
------------------------------

- Initialize a new repository

        $ mkdir myproject
        $ cd myproject
        $ git init
        Initialized empty Git repository in /home/krinkle/myproject/.git/
        $

- Check on your status (initially empty)

        $ git status
        # On branch master
        #
        # Initial commit
        #
        nothing to commit (create/copy files and use "git add" to track)
        $

- Make some files

        $ echo "My first file" > newfile.txt
        $

- Check on your status

        $ git status
        # On branch master
        #
        # Initial commit
        #
        # Untracked files:
        #   (use "git add <file>..." to include in what will be committed)
        #
        # newfile.txt
        nothing added to commit but untracked files present (use "git add" to track)
        $

Stages of a file
----------------

- A file starts its life as an **Untracked file**.
    We call it Untracked, but it is still kind of tracked in the sense that git knows it is has come into existence.
    It just won't track changes in the contents of the file.

- We need to explicitly tell git that we care about this file, that we want to know what happens to it.
    This happens when we add the file to the **staging area**.
    The **staging area** is a snapshot of what we are going to commit when we are ready.

- A **commit** is a snapshot of the current point in history, marked by a message describing what was changed since the last commit
  and a SHA1 hash to distinguish it from other commits. These are the scrolls that we are going to study when we look back in time.

Add to the staging area
-----------------------

- Adding something to the staging area is like saving a temporary snapshot. We can continue to edit the file, and git will continue to track the changes since we last added the file to the staging area.

        $ git add newfile.txt
        $ git status
        # On branch master
        #
        # Initial commit
        #
        # Changes to be committed:
        #   (use "git rm --cached <file>..." to unstage)
        #
        # new file:   newfile.txt
        #
        $

- You can keep adding files to the staging area until you are ready to make a commit.

Committing
----------

- Now is the time to make history. Making a commit will copy the snapshot currently in your staging area and mark it with a descriptive message and an identifying hash.

        $ git commit

- This command will open the editor you set in the beginning of the workshop. Write a descriptive message. Your message can span multiple lines, but if you have a multiple-line commit message the first line needs to be separated from the rest by an empty line. This is a kind of subject / message format.
- If you don't want to use an editor you can make a commit message via the command line. This only supports one-line commits.

        $ git commit -m "Descriptive message"
        [master (root-commit) f2240c8] Descriptive message
         1 file changed, 1 insertion(+)
         create mode 100644 newfile.txt
        $ git status
        # On branch master
        nothing to commit, working directory clean
        $

Reviewing History
-----------------

    $ git log
    commit f2240c81da50f348bf6523c46134ac3f4ca7a062
    Author: Colleen Murphy <cmurphy@cat.pdx.edu>
    Date:   Fri Apr 11 14:37:02 2014 -0700

        Descriptive message
    $

- Notice the Author section. You configured this early on in this workshop.

Getting the Rhythm
------------------

- This is the pattern for how most of your git career will go. Get comfortable with it. (The `git status` isn't necessarily part of the pattern but it is good to follow what is going on for the moment).

        $ # edit newfile.txt
        $ git status
        $ git add newfile.txt
        $ git status
        $ git commit
        $ git status

- Here's the shortcut:

        $ git commit -a   # Automatically adds all tracked changes to the staging area before committing

Seeing the differences
----------------------

- If you are unsure what changes you made at any point in time, you can see them:

        $ git diff                     # Tracked files that have been modified but not staged
        $ git diff --cached            # Files in the staging area
        $ git log -p                   # Changes throughout your commit history

Other ways to put changes in the staging area
---------------------------------------------

    $ git rm newfile.txt                              # Remove the file
    $ git mv newfile.txt reallynewfile.txt            # Rename the file

Branching
---------

- One of git's best features is the way it handles branching. Branching allows you diverge from your main project to work on a feature or bug without altering the main project. When your feature or bugfix is ready, you can merge your work into the main project.
- View your current branch

        $ git branch
        * master
        $

- You start out on a branch called master. This is analogous to trunk in SVN.
- Create a new branch

        $ git branch testing
        $ git branch
        * master
          testing
        $

### Switching branches

    $ git checkout testing
    Switched to branch 'testing'
    $ git branch
      master
    * testing
    $

- Realistically, whenever we want to create a new branch, we also want to immediately start work on it. We can shortcut the steps shown previously with

        $ git checkout -b testing
        Switched to a new branch 'testing'
        $

- Feel the rhythm

        $ echo "testing" > testfile.txt
        $ git add testfile.txt
        $ git commit

- Check on your history in your testing branch

        $ git log

- Switch back to your original branch

        $ git checkout master
        Switched to branch 'master'
        $

- View the history of the orinal branch. How many commits do you have?

- List the files in your current directory with `ls`. Is testfile.txt there?

- Switch back to your testing branch. Make sure testfile.txt is there.

Merging
-------

- If you are satisfied with the changes you made to your testing branch. it's time to merge them into master

        $ git checkout master
        Switched to branch 'master'
        $ git merge testing
        Updating f2240c8..3addaec
        Fast-forward
         testfile.txt | 1 +
          1 file changed, 1 insertion(+)
           create mode 100644 testfile.txt
        $ git log
        commit 3addaec5086875e1a59d1b98e4f9e334bc0828e5
        Author: Colleen Murphy <cmurphy@cat.pdx.edu>
        Date:   Fri Apr 11 16:26:03 2014 -0700

           testing

        commit f2240c81da50f348bf6523c46134ac3f4ca7a062
        Author: Colleen Murphy <cmurphy@cat.pdx.edu>
        Date:   Fri Apr 11 14:37:02 2014 -0700

           Descriptive message

Resolving Merge Conflicts
-------------------------

- Merge conflicts can be the most difficult thing about git. First let's simulate a merge conflict:

        $ git branch
        * master
        $ git checkout -b newbranch
        echo "derp" > derpfile
        $ git add derpfile
        $ git commit -m "derp commit"
        $ git checkout master
        $ echo "nonderp" > derpfile
        $ git add derpfile
        $ git commit -m "non derp commit"
        $ git merge derp

- At this point you have made two changes to the same line of the same file, in two different branches. Git does not know which change you mean to keep in master. If you run `git status` it will tell you which problem file has been modified on both branches. Open it in your editor. You should see something like this:

        <<<<<<< HEAD
        non derp
        =======
        derp
        >>>>>>> newbranch

- The part between <<<<<<< HEAD and ======= is the part of your commit that comes from your current branch. The part between ======= and >>>>>>> newbranch comes from the branch you are trying to merge in. Pick which line you want to keep, then delete the other line and all of the separators. Save and quit. Then `git commit` and save the merge message it gives you.

Intro to Github
---------------

- Visit http://github.com and create an account.
- Click the '+' in the upper right corner. Click Create New Repository from that dropdown.
- Give it a name and a description. Don't click the "Initialize this repository with a README" checkbox.
- Follow the instructions to "Push an existing repository from the command line"

Remotes
-------

- A remote is a foreign location. This will be the identifier for your remote repository.

        $ git remote add origin git@github.com/cmurphy/myproject.git
        $ git remote -v
        origin  git@github.com:cmurphy/test.git (fetch)
        origin  git@github.com:cmurphy/test.git (push)
        $

- Listing your remotes will give you two URLs, one to get data from and one to push data to. By default these are the same.
- It is common to name your main remote 'origin' but this is not required.

Pushing to Github
-----------------

- Pushing means copying your local repository over to a remote one.

        $ git push -u origin master

- origin is your remote repository. master is the branch you want to push.
- -u tells git to set this origin as upstream, which means in the future you can just say `git push`

Working with an existing project
--------------------------------

- Often you will want to contribute to a project that is already on Github. If you are a registered **contributor**
  to the project. you will be able to make changes directly to the repository. Otherwise, you will have to **fork**
  the repository and ask the maintainers to merge your changes. This is called making a **pull request**.
- Visit https://github.com/pdxcat/nixmentors. Click the button in the upper right labeled "Fork".
- Notice the namespace changes from pdxcat to your username.
- This fork will NOT automatically update when **upstream** (the pdxcat version) changes. You need to keep it updated yourself.

Pull Requesting
---------------

- Get a copy of your forked repository. There should be a "clone URL" on the right side of the page. Copy the HTTPS version if you have not added
  SSH keys to Github yet. You can use the SSH version if you have uploaded SSH keys. The Subversion button doesn't exist.

        $ git clone https://github.com/cmurphy/nixmentors.git
        Cloning into 'nixmentors'...
        remote: Reusing existing pack: 495, done.
        remote: Counting objects: 12, done.
        remote: Compressing objects: 100% (9/9), done.
        remote: Total 507 (delta 4), reused 0 (delta 0)
        Receiving objects: 100% (507/507), 118.02 KiB, done.
        Resolving deltas: 100% (234/234), done.
        $ cd nixmentors
        $ ls

- Set the original repository as "upstream" so you can keep your copy up-to-date

        $ git remote add upstream https://github.com/pdxcat/nixmentors.git
        $ git remote -v
        origin  https://github.com/cmurphy/nixmentors.git (fetch)
        origin  https://github.com/cmurphy/nixmentors.git (push)
        upstream  https://github.com/pdxcat/nixmentors.git (fetch)
        upstream  https://github.com/pdxcat/nixmentors.git (push)
        $ 

- Any time you are going to work on new changes, you should pull from upstream before you start your work. This will help reduce merge conflicts later.

        $ git pull upstream master

- Often we checkout a new branch to make it clear what we're working on, but you can also do you work on master.

        $ git checkout -b fix_lab2
        $ # Make changes
        $ # git add changes
        $ # git commit changes with descriptive message
        $ git push origin fix_lab2

- Now use the Github UI to ask for your changes to be merged.
  - Your fork should now have a "Compare & pull request" button. Click it.
  - Write a comment about your change. You should include what the change is, and any documented issues/bugs that this change addresses.
  - Pay attention to the note on the right side of the comment section. If your branch is not able to merge cleanly, the maintainer will likely ask you to fix your conflicts.
  - Click the "Send pull request" button.
  - If/when the maintainer accepts your pull request, you will need to update your master branch by pulling from upstream again before you start more work.

Git rebase
----------

- Sometimes after you have made a number of commits, you need to change your history. You may need to reword a commit, or change the order of commits, or combine multiple commits into one commit. This is what git rebase is for.
- Scenario 1: You need to reword a commit you made three commits ago.

        $ git log  # check your commit history, count how many commits before the current commit the problem commit occurs (three).
        $ git rebase -i HEAD~3  # -i means interactive mode; HEAD~3 means go back three commits before the current commit on the current branch

  - You will be presented with a text editor containing your last three commits. On the line for the commit you want to change, replace 'pick' with 'reword'. Save and exit.
  - The commit will be opened in another editor. You can edit the commit here. Save and quit again.
  - git log to see your change.
- Scenario 2: You want to swap the order of two commits.
  - Do another interactive rebase
  - Swap the order of the two commits in the editor that opens up. Save and quit.
- Scenario 3: You need to combine multiple commits into one commit message. This comes up a lot in the CAT, where we need to push committed code to a testing branch in order to deploy it, and then later need to rewrite our commit messages so that they succinctly sum up the purpose of the code. This doesn't necessarily happen a lot in other environments, since every time you make a commit it should be for one small change.
  - Interactive rebase, going back as long as you need to encompass all the commits you want to combine.
  - The action for the commit you want to keep will either be 'pick', if you like the commit message, or 'reword' if you want to change the wording
  - The action for the commits you want to combine with the main commit will either be 'squash' or 'fixup'. 'squash' will insert the commit message into the new commit. 'fixup' will discard the message.
  - In any case, you are not losing your work. The work from the squashed/fixedup commits will be merged into the main commit.
  - After you finish your rebase, run `git log -p` to see that all of your changes are now under your main commit.
- Scenario 4: You are working on a test branch, but since you started working, the master branch has changed. You need to keep your branch updated.

        $ git fetch origin master   # fetches changes from the remote without merging them into your local repo
        $ git rebase origin/master  # merges changes from master, placing your changes at the top; notice no -i

Making Mistakes
---------------

- Git makes is possible to recover from mistakes, even when git caused them.

### Amending a commit

- You can fix your last commit message

        $ git commit --amend

- You can also add new changes to your last commit

        $ git add newfile
        $ git commit --amend

### Undo all unstaged changes to a file

    $ git checkout file    # restores the file to the state it was at at the last commit. This is very destructive! You cannot recover from this!

- Git will recommend using the syntax `git checkout -- file`. The `--` is to protect you from accidentally adding flags to git checkout if the file name contains a `-`. If this doesn't make sense, it is just safer to use `git checkout -- file`.

### Git Reset

- git reset restores the repo to a previous state.

- Scenario 1: You have added a file to the staging area that you did not mean to add. You can unstage it with:

        $ git reset HEAD badfile

  - You will not lose your changes to the file. It has just moved out of the staging area.

- Scenario 2: You have committed two changes and you want to undo the commit without undoing the work.

        $ git reset --soft HEAD~2  # Undo the last two commits, keeping the work that you did in the staging area.

  - From here you can reorganize your commits, reword your commits, unstage files, or discard changes completely (using git checkout)

- Scenario 3: You have committed a change and you want to undo the commit and all of the work that you did with it.

        $ git reset --hard HEAD^  # Undo the last commit, discarding all changes

  - Note the syntax HEAD^, which is equivalent to HEAD~1.

### Git Revert

- The problem with changing your commit history via `git commit --amend` or `git reset` is that, if you have already pushed your commits to a remote repository, your repo may have been used by someone else. Changing history completely breaks their ability to re-pull or to contribute back changes. If you have already publicized your mistakes, it is best to admit to it and fix it publicly.

        $ git log  # Find the hash of the commit you want to revert
        $ git revert <hash>   # This will add a commit describing that you reverted a previous commit

- Never `git push -f` (force push) to the master branch of a repository. If you feel like you need to, you need to rethink how to undo your mistake.

### Git Reflog

- If everything else fails and you have completely lost track of what you were trying to do, git reflog is the answer. `git reflog` shows the state of the repository at every point that you made some sort of change with git: staging files, committing things, merging things, checking out different branches. 

        $ git reflog    # See what you've been doing
        $ git checkout HEAD@{2}  # Go to whatever state you were in two git commands ago (like, prior to committing something)
        $ git checkout -b newbranch  # After checking out HEAD@{2} you will be in a "headless state". You need to create a branch to work on from here.

Other Useful Commands
---------------------

- git blame

        $ git blame file.txt   # Provides line-by-line documentation for who made what commit

- git grep

        $ git grep searchterm  # Searches the repository recursively starting from your current directory

- git stash

        $ git stash            # Saves your current modifications without committing anything, so you can go to an unmodified state or switch to a different branch
        $ git stash pop        # Get your stashed changes back

Git and Github Etiquette
------------------------

- Commit messages should be in the imperative, i.e. "Add feature", not "Added feature" or "Adds feature".
- Commit messages have a subject that should be under 80 characters and optionally a longer message, separated by a newline from the subject, also wrapped to 80 characters per line.
- Never force push to a master branch or any branch that anyone may be working on. Use git revert to backtrack your mistakes on public branches.
- Be polite when working with contributors or maintainers. You do not want to be [this guy](https://gist.github.com/uppfinnarn/9956023).
- For more about etiquette:
  - http://jamiethepiper.com/git-commit-good-etiquette/
  - http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html

<!--
Other things to cover:

git cherry-pick
git tag

Advanced/miscellaneous
.gitignore
aliases, other configs
additional git commands (git-thing in path)
ssh keys

--->
More
----

Git is a complex beast. There are always more commands, subcommands, flags, and shortcuts to learn. For an excellent reference, try http://git-scm.com/book
