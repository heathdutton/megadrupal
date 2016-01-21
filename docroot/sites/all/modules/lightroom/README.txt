Install the DrupalPublish plugin in Adobe Lightroom with the Plug-In Manager.
The Plug-In Manager can be found under the File menu.

Edit the existing Drupal Publish settings, or create a new Drupal Publish
service. Under Drupal Settings, set URL to the base path of your Drupal
installation, with trailing slash (e.g. http://www.example.com/). Enter your
Drupal user name and password.

Enable the Lightroom module in Drupal. The module uses Features to configure
the following resources:
- a content type (Collection) and field (field_collection_images) to store
  images published from Lightroom
- an API endpoint, with select services enabled, that allows Lightroom to
  connect to your Drupal web site. The services include:
  - Node CRUD operations
  - File CRUD operations
  - User login
  - A custom node type information action

Once the Lightroom plugin and Drupal modules are installed and configure, create
new collections in the newly created Publish Service in Lightroom. Add images,
reorder images, and republish the collection, and your changes will appear in
the node on your website.

Other node types can be managed with Lightroom; just create a new content type
with the 'field_collection_images' field. When creating a new collection in
Lightroom, select the node type that you want from the Create Node dialog.