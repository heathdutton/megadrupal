The module provides an integration between Jira (via the jira_rest
module) and rules.

In the current limited form the module provides a rules action for
creating simple, basic issues in Jira.

You must configure the jira_rest module with username and password at
`admin/config/services/jira_rest`.

In the rules action you can configure:

 - Project key
 - Issuetype name
 - Summary
 - Description

After successfully creating the issue the action provides the issue ID,
issue key, and a link to the REST API endpoint of the newly created
issue.
