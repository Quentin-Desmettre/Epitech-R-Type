# Epitech - R-Type Contribution guidelines

In this file, you will find the contribution guidelines for the R-Type project.

# Table of contents

- [Git workflow](#git-workflow)
  - [Branches](#branches)
  - [Commits](#commits)
- [Code style](#code-style)
- [GitHub workflow](#github-workflow)

# Git workflow

## Branches

The `main` branch is protected, meaning that you cannot push directly to it.
To add new features or fix bugs, you must create a new branch from `main` and open a pull request to the `dev` branch.

Branches must be named using the following convention:

- `feature-<feature-name>` for new features
- `fix-<bug-name>` for bug fixes
- `refactor-<refactor-name>` for refactors
- `doc-<doc-name>` for documentation
- `test-<test-name>` for tests
- `misc-<misc-name>` for anything else (make sure to explain what it is in the pull request)

> If a pull request is made directly to `main`, and does not come from `dev`, it will be automatically rejected.
> If a branch does not follow the naming convention, it will be automatically rejected.


## Commits

Commits must be atomic, meaning that they must only contain changes related to a single feature, bug fix, refactor, etc.

Commits must be named using [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/):

- `feat(<scope>): <description>` for new features
- `fix(<scope>): <description>` for bug fixes
- `refactor(<scope>): <description>` for refactors
- `docs(<scope>): <description>` for documentation
- `test(<scope>): <description>` for tests
- `chore(<scope>): <description>` for anything else (make sure to explain what it is in the commit message)

For instance, a commit that adds a new feature to the rtype client would be named `feat(client): added flying saucer, a new enemy`.

> If a commit does not follow the naming convention, the pull request will be automatically rejected.

# Code style

The code style is enforced using [clang-format](https://clang.llvm.org/docs/ClangFormat.html).
At the root of the repository, you will find a `.clang-format` file that contains the style configuration.

This style is inspired by the [LLVM style](https://llvm.org/docs/CodingStandards.html#clang-format-style-options), and customized to fit our needs.

You can run the script `scripts/clang-reformat.sh` to reformat the whole project.

> If a pull request does not follow the code style, it will be automatically rejected.

# GitHub workflow

The GitHub workflow is based on GitHub Actions.

3 workflows are defined:

- Push to `main`: this workflow is triggered when a pull request has been merged into `main`.
  It builds the project, and if the build succeeds, it creates a new release, and push the repository content to a mirror repository on GitHub.
- Pull request to `main`: this workflow is triggered when a pull request is opened to `main`.
  It builds the project, and if the build succeeds, it runs the tests and the linter, and tries to create a new release.
  This workflow must pass before a pull request can be merged.
- Push to `dev`: this workflow is triggered when a pull request has been merged into `dev`.
  It performs the same check as the `Pull request to main` workflow, but does not try to create a new release.
