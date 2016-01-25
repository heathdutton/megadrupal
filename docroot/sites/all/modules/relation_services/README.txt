This module provides integration between Relation module and Services module.
After enabling it you will be able enable relation data as resources in
services endpoints. One additional use case is to deploy relations between
sites with Deployment module.

In order to be able to deploy relations you will need:
 * Relation UUID module [1].
 * Patch #1 [2] from #2141971 [3] should be applied to module as well.
 * Patch #1 [4] from #2146005 [5] should be applied to Relation [6] module.
 * Patch #1 [7] from #2146929 [8] should be applied to Relation [9] module.

[1] https://drupal.org/sandbox/Mywebmaster/2075541
[2] https://drupal.org/files/issues/relation_uuid.code-2141971-1.patch
[3] https://drupal.org/node/2141971
[4] https://drupal.org/files/issues/relation.code-2146005-1.patch
[5] https://drupal.org/node/2146005
[6] https://drupal.org/project/relation
[7] https://drupal.org/files/issues/relation.api-2146929-1.patch
[8] https://drupal.org/node/2146929
[9] https://drupal.org/project/relation
