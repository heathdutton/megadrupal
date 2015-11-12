This module provides a function to check new content which may be similar or equal to other nodes. This does integrate with cck and uses phonem to make sound check on strings to detect "drupal" and "trupal" as equal field content. You can set the fields that are used too check if the node is unique or if it already exists in admin setting area. The module lets you choose which compare method you would like 
to use, equality or phonemic sound check (with phonem 'drupal' is equal to 'trupal' because it sounds similar).

--- Installation ---
Just place the module into your sites module folder, in most cases /sites/all/modules, and enable it on the admin  module page /admin/build/modules.

--- Configuration and usage ---
Go to your content-type in drupals adminsettings and go to the settings of the field you want to use to check if a node is unique. You can select as many fields as you want. For example if you have a content type 'person' with fields 'firstname', 'lastname', 'birthday', firstname and lastname could be checked for equality to decide if a node that represents the person already exists. To get this done, go to the edit page of the field 'firstname' and select an option in the fieldset 'unique content' at the bottom of the page.
You have 3 options and one checkbox:

- Don't use for unique check (self explanatory -> This field is not used to check for equality)
- Compare method: Exact equality (matches for 'drupal' = 'drupal' -> only exact matching values)
- Compare method: phonemic (matches for 'drupal' = 'trupal' -> phonem sound check is used)

The checkbox 'only same node type' checks this field only for nodes of the same type. This is, because you can use one and the same field for multiple nodetypes. If you do not check this box, nodes of different node types may be equal if the fields, you have chosen, match. You can do almost the same for nodes of the same user.

Now, if you have set all fields to be checked as you'd like, go to the edit form of you content type.
On this page, bs_uniquenode gives you some more options to set how non unique content is handled. These options should be selfexplanatory with the given description. You can set a message to be displayed if a node seems to be a doublette and in the fieldset 'Checked Fields' you can see which fields are used to chek for doublettes.
You can even check all available field options for the title field, which is not a cck field but always available on each node type. In our example it could be neccessary to check the title if it represents the lastname of our person-node.

--- Building the phonem index ---
The module builds a phonemic sound index. This is done while adding new nodes (after insert). If you add this module to you existing Drupal installation, make sure cron (cron.php) is running. The cron will build your phonem index on fields you have chosen to be compared using phonem if nodes of this type already exist. If there is no phonem index, fields will only be matched for exact equality.

--- Maintainer ---
The maintainer of this module is manuelBS
