CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Using Workbench Moderation Profile Module
 * Integration
 * Dependencies


INTRODUCTION
------------

Current Maintainer: Joshua Turton <srjosh@srjoshinteractive.com>

Workbench Moderation Profile adds the ability to have multiple workflows,
divided into profiles, based on the states and transitions defined by Workbench
Moderation. These workflow profiles can then be applied to nodes based on
criteria you determine, and an API is included to help with that process.


INSTALLATION
------------

Workbench Moderation Profile relies on the Workbench Moderation module. It does
require that transitions be entities, which happened in the dev version circa
1 July 2014.


USING WORKBENCH MODERATION PROFILE MODULE
-----------------------------------------

Workbench Moderation Profile does the following things by itself:

1.  Provides an API that allows for the application of those profiles to nodes.

It does NOT do the following:

1.  Provide ANY functionality that actually applies those profiles to nodes.
    That's your job.

Fortunately, Workbench Moderation Profile comes with two submodules,
Workbench Moderation Profile Node and Workbench Moderation Profile OG, which
allow you to define profiles of transitions by content type and Organic Group,
respectively.


INTEGRATION
-----------

There are two primary parts to this integration: Assigning a profile, and
applying that profile's transitions to your node workflow.  

The first part is up to you - this can take many forms as profiles are designed
to work however you need them to.  See workbench_moderation_profile_node for an
example of how to assign a profile to a content type. It's usually some form of
form_alter function, as you are adding it on to some other configuration form.
Ultimately, you will need to determine how this works for you and go with it.

The second part's pretty easy - telling workbench_moderation_profile module 
which profile to use for each node.  This is accomplished using a single hook
function, hook_workbench_moderation_profile_get_profile().  This function should 
return the profile unique key (wmpid) as an integer. WMP then uses this profile
to override the available states for workbench_moderation forms. For more
details, see workbench_moderation_profile.api.php.


DEPENDENCIES
------------

Required for the transition entity reference field.
1. ctools
2. entity
3. entityreference
4. options

Required for baseline workbench functionality.
5. workbench
6. workbench_moderation
