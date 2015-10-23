BIOLIFE THEME BY NOOD.ORG

This theme is a sub theme of Noodle base theme (http://drupal.org/project/noodle). You have to install Noodle first (but you don't have to enable it).

The theme only wraps the setup and content you already have, but since many people may ask how to make a theme look exactly as demo site (http://biolife.themes.nood.org/) we offer these instructions and tips:

MENUS: 

The Biolife doesn't support Drupal menu variable (Drupal 8 way) so you just have to enable core menu block and place it in: 
- Main menu would usually go to 'header navigation' region.
- Secondary menu would usually sit in any of sidebar regions.

Biolife also has styles to support Menu block (http://drupal.org/project/menu_block) and Superfish (http://drupal.org/project/superfish) modules in 'header navigation' block region, so again just place the block there.

FRONT PAGE SLIDESHOW:

To learn how you make views slideshow, you can check our blog post (http://nood.org/blog/how-create-views-slideshow-drupal-7) or many other tutorials available. Unless you want to modify CSS you have to give your view:
- Class 'front-slideshow' (edit view -> advanced -> CSS class).
- Image size is 500x275.
- Node title field should be placed below image in view.
- The text to the left of the image is views footer.

NEWS VIEWS:

Tips how to make news views for both front page and news page:
- Unless your view is called 'news' you should give it a 'view-news' class (edit view -> advanced -> CSS class).
- News page is easy: it is teaser node list view of 'news' content type  (so you need to create 'news' content type first).
- Smaller image on node list is set up in 'display setting' for teaser of 'news' node type. But of course you have to have convenient image style to do this.
- Front page view is a field list block view (fields are: image (325x150px), title (set to h3), submitted, teaser (set to trim at 150 characters), node link, pager set to display only 4 items).
- Place the news block only for front page in content bottom region.


QUOTATION FIELD:

- Just make a text field and call it 'brief'. Then you can place it anywhere in the node. Alternatively, you just can use <blockquote> tag in your text.


IMAGES:

- png files named as zzz-source.png are Fireworks source files (sorry PS folks, Fireworks is a tool of choice since 1998).