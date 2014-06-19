# SPAOP `gh-pages` branch

If you are reading this README, you are on the `gh-pages` branch of the SPAOP repository. This branch is what Github takes and hosts under [https://martinhh.github.io/SPAOP](https://martinhh.github.io/SPAOP) (read more about Github Pages [here](https://pages.github.com)).

To be able to auto-generate Doxygen pages right into this branch, it was included as a submodule (resulting in a recursive repository structure(!)). The path to this README should look something like this:

`/SPAOP[SPAOP master branch]/Doxygen/html[SPAOP gh-pages branch]/README.md`

Make sure only html stuff is commited into this branch. It is strongly recommended to never ever switch to or from this branch using `git checkout` - the only way to switch to this branch should be by entering the `html` directory. 