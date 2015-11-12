release_tag=7.x-0.5-rc0
release_branch=7.x-0.5
dev_branch=0.5.x
semver_tag=0.5.0
username=ergonlogic

# Clone the release repo from drupal.org into a temporary directory
mkdir tmp && cd tmp
git clone $username@git.drupal.org:project/valkyrie.git --recursive
cd valkyrie

# Add upstream remote and fetch relevant branch
git remote add github https://github.com/getvalkyrie/valkyrie.git
git fetch github $dev_branch
git checkout $dev_branch

# Checkout or create the d.o release branch
git checkout -B $release_branch

# Merge changes and update submodules
git merge -s ours github/$dev_branch
git submodule update --init --recursive

# Exit if something is amiss
git status | grep "nothing to commit, working directory clean"
if (( $? > 0 )); then
  exit 1
fi

# Match all our submodule paths
grep path .gitmodules | sed 's/.*= //' | while read submodule; do
  echo "Unregistering submodule at: $submodule"
  git rm --cached $submodule
  rm -rf $submodule/.git
  git add $submodule
done

echo "Removing submodule registry: .gitmodules"
git mv .gitmodules gitmodules.bak

echo "Committing collapsed submodules."
git commit -m "Collapse submodules."

echo "Tagging with $release_tag, and pushing tag to drupal.org."
git tag $release_tag
git push origin $release_tag

echo "Restoring submodules, and pushing to drupal.org."
git revert HEAD --no-edit
git push origin $release_branch

echo "Removing temporary directories."
cd ../.. && rm -rf tmp

echo "Tagging with SemVer tag ($semver_tag) on dev repo."
git tag $semver_tag
git push origin $semver_tag
