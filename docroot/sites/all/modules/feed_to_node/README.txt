This module is created to provide an easy to setup and go feed reader.

At the moment it's created to read default RSS feeds and the Youtube api/feed.
The module allows you to setup a template node with the use of tokens.
Text, taxonomy and image fields can be used with the tokens.
The template node will be used to create nodes for each item that is loaded.

I tried to keep the module as basic and simple as possible.
You can also make your own parser with only 4 functions, 
check the Youtube or RSS module to see how.

This module needs the Token module to run.

How to use:
Go to admin -> Configuration -> Manage feeds to nodes
Here you find an overview of your current feeds
To get started you first need to select which content types can 
be used as feeds.

1.Go to config tab, this will show an overview of all available content types.
  Check the content types you plan to use as feeds.

2.Now we got one or multiple content types selected we can create a template
  for our feed. go "Add content" and create a new node based on the content type
  you just selected.
  If you like at the node form, you will hopefully notice the token list,
  this will either be a list of available tokens if you got 1 parser installed
  or a list of parsers which can be clicked open.

  Click on the token list you want to use, this can be RSS feed, Youtube feed
  or any other if you got more parsers installed.
  Place the tokens in the fields where you want them.
  When Feed to node runs it will replace all the tokens with info from the feed.
  
  When done placing the tokens, make sure the unpublish the node, you probally
  don't want to publish a node full of token fields.
  Hit save, to save the node.
  Look up the node id, by going to the edit screen of the node. 
  Remember the node ID because we need the ID in the next step.

3.Now go back to admin -> Configuration -> Manage feeds to nodes
  We can now create the feed we want.
  Click on "Add feed to node".
  Give the feed a name which would give a good description of your feed.
  Choose a Parser, depending on the data you want to call.
  - RSS can Parse normal RSS feeds
  - Youtube can Parse data from the Youtube api.
  Now enter or copy paste the feeds url. 
  WARNING: make sure to read any description below the Feed url, it could be
  that you need to use specific settings for the feed to work.
  Next we need to give a Node ID, this is the node ID that acts as template.
  This is the node you created in step 2 just now.
  
  Check "Always published" to make sure all the nodes created are all published.
  Check "Active" if you want your feed active right away.
  Hit "Submit" to save the feed.

4.Congrats you just created a feed. You can edit a feed by going to 
  admin -> Configuration -> Manage feeds to nodes and clicking on a feed.
  Here you will notice an extra tab called "Feed list", this will show all nodes
  created by the feed.

5.Feed to node is linked to drupal cron, that means you have to use drupals cron
  to load you feeds and manage the refresh period.
