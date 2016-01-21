## Similar By References ##
- - - - - - - - - - - - - - - - - -

This is an adaptation of [Similar By Terms](http://drupal.org/project/similarterms) to use entityreference fields for determining similarity instead of taxonomy. The module creates lists of nodes sorted by their similarity to a given node based on commonality of values in an entityreference field. It uses Views to create its database queries and display its results. This means that you can define what fields to show, additional sorting, filters, displays, etc.

### Set up: ###

The simplest way to set things up is to add an entityreference field to the nodes which you'd like to show similarity and then add a number of related values to your content. 

Once you have identified the entityreference field that will be used to compare similarity, visit admin/build/views. You'll find a view called "Similar By References". You can override the default view to customize the block or clone it if you want to create a new view.

Edit the view. Expand the 'Advanced' section to find the settings for contextual filters. Edit the 'Similar By References: Entityreferences in Content' filter, and look for the option 'Limit similarity to references from these fields', where you can select the entityreference field that the filter should use. Save the filter and the view with these changes.

Then visit admin/build/block add the 'Similar By References' Block to the page. This block will *only* appear on node pages and it will list other content similar to this node based on the value in an entityreference field. The block will also not appear if the node has no entityreference values associated with it.

### Views integration: ###

If you would like to customize this block or create a new view listing content by similarity, 
- **Contextual filters** - Similar to what? First off, Similar By References needs to know what node it is going to show stuff similar to. This is done through the "Contextual filters" section of the Views interface. Adding *"Similar By References: Entityreferences in Content"* here will allow you to pass in a node id. You can usually set this to 'Provide default argument' > 'Content ID from URL' if you want to show a block on the same page as a node, or a tab on the node page. The settings here will also allow you to limit similarity to values in specific entityreference fields and you can choose whether to show the original node in the listing or not.

- **Sort criteria** - Similar By References' magic happens in the "Sort criteria". Simply add *"Similar By References: Similarity"* as your first sort criteria and results will be sorted by similarity to the node given as an argument to the view. Set sort order to "Descending" to show most similar stuff at the top. You may also want to provide secondary sorting, so that nodes with the same number of common terms are sorted either alphabetically, or by date.

- **Fields** - There's also a field titled *"Similar By References: Similarity"* which shows the commonality between the values associated with the listed nodes and those of the node being passed in as an argument. This can be output either as a percentage (recommended), or as a raw count of the number of terms which the nodes have in common. This field is particularly useful in testing to make sure that you've got everything set up correctly.

Keep in mind that the more values used, the better for finding similarity. However, more values will create slower database queries. Be sure to cache blocks and Views output where possible.

### Example ###

    Given node (passed as argument):
      Node 1
        References: A, B, C, D, E, F
        (6 references total)
  
    Results:
      Node 2
        References: [B, C, D, E, F,] G
        (5 references in common with given = similarity 83%)
      Node 3
        References: [A, B, C, D, E,] H
        (5 references in common  = similarity 83%)
      Node 4
        References: [B, C, D, F,] I, J, K, L, M
        (4 references in common = similarity 67%)
      Node 5
        References: [A, B, C]
        (3 references in common = similarity 50%)
      Node 6
        References: [A, B,] J, K, L, M, N, O, P, Q
        (2 references in common = similarity 33%)
        

### Other Notes ###
Since this is a Views plug-in, it is possible to create listings of related images, videos, or just about anything which you can show with Views.

This module has been designed to be used with any type of entity supported by the entityreference field. Entities other than nodes are not heavily tested yet. Please report problems on the project issue queue.