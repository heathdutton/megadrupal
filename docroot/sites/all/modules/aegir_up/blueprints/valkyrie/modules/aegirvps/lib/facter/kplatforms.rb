kplatforms_git_url = 'http://git.drupal.org/project/kplatforms.git'
# We do the expensive lookup once, and set it as a fact. For Facter 1.7+ we'll
# be able to cache this value, so we only need to check once a day, by using:
# Facter.add("kplatforms_latest", :ttl => 86400) do
Facter.add("kplatforms_latest") do
    setcode do
        # Get all the tags from the kplatforms git repo (as a chunk of text)
        all_tags = Facter::Util::Resolution.exec("git ls-remote --tags #{kplatforms_git_url}")
        # Split the text into an array, at the ends of lines, and return the
        # last one, which should be the most recent tag.
        all_tags.split("\n").last
    end
end
Facter.add("kplatforms_tag") do
    setcode do
        # Get the latest tag from our custom fact
        latest = Facter.value('kplatforms_latest')
        # Split the text into an array, delimited by 'refs/tags/', which
        # precedes the tag. Then we can just return the last element, as it
        # should contain the tag string.
        latest.split("refs/tags/").last
    end
end
Facter.add("kplatforms_release") do
    # We need to add this fact since we can't use split() in Puppet 2.6.2
    setcode do
        # Get the latest tag from our custom fact
        release = Facter.value('kplatforms_tag')
        # Split the text into an array, delimited by '-', which precedes the
        # release version. Then we can just return the last element, as it
        # should contain the version string.
        release.split("-").last
    end
end
Facter.add("kplatforms_commit") do
    setcode do
        # Get the latest tag from our custom fact
        commit = Facter.value('kplatforms_latest')
        # Split the text into an array at the spaces separating the commit hash
        # from the tag. Then we can just return the first element, as it should
        # be the commit hash.
        commit.split(" ").first
    end
end

