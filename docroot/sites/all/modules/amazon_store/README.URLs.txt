Id: README.URLs.txt,v 1.2 2009/04/28 22:10:00 rfay Exp $
Explanation of URLs and Browsenodes in the Search URL

When you land on the /amazon_store page with the no keywords, a BrowseNode is selected for you, depending
on the selected SearchIndex. If you have a SearchIndex selected, the browsenode is looked up and
results from that browsenode are shown. If you have no keywords, no searchindex, and no browsenode, it
just puts the keywords "Featured" and searches for it in searchIndex "All". Probably can do better than this.

Other items that can go on the URL:
MinimumPrice=
MaximumPrice=
Brand=
SearchIndex=
Keywords=
Sort=  (try the sorts you see as you try out the interface)
BrowseNode=  (a numeric browsenode - find what you're looking for on browsenode.com)
ItemIid=  (a comma-delimited list of ASINs)

If you want a page with a specific set of things, without a search, you can just choose a BrowseNode:
http://example.com/amazon_store?BrowseNode=xxxxx
or
http://example.com/amazon_store?ItemId=ASIN1,ASIN2,ASIN3,ASIN4,ASIN5

You can look at browsenodes at http://browsenodes.com



