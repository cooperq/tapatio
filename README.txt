DESCRIPTION
===============================================================================
Enable users to leverage a drupal system as a front end to twitter.com (and 
other sources in the future) allowing them to use drupal as a hub for message 
aggregation, moderation, and dispatching.

FEATURES
===============================================================================
* Tight integration with twitter.
    Operators, users with access to the drupal system, associate twitter 
    accounts with organic groups.  All tweets from followers of those accounts 
    will automatically get pulled into the drupal system as nodes.

* Communications moderation
    Operators will then add and moderate posts, voteing on them (automatically 
    dispatching at a configureable value), specifying priority levels, 
    associating with other groups (twitter accounts), marking as a duplicate of 
    another post, and dispatching.

* Detailed search / display interface
    Operators also have the abilty of searching for nodes by minutes since last 
    recieved, wether or not the node has been dispatched, the current vote 
    level, and wether or not the node has been prioritized.

* SMS Dispatching through twitter
    After a operator deems that a node is valid (ie. it has been verified by an 
    alternate source, it has been assigned a SMS message, and it has been 
    associated with a group) he or she can dispatch it.  This is essentially 
    posting the sms message of the node as a tweet for each twitter account 
    (group) that is associated with the node.

INSTALLATION
===============================================================================
It is recommended that you install this module through the tapatio install 
profille (http://rnc08coms.hackbloc.org/download).  However if you chose to 
install this module manually you will need to take the following steps:
- Download and install the required modules: userpoints
(http://drupal.org/project/userpoints), og (http://drupal.org/project/og),
views (http://drupal.org/project/views), votingapi 
(http://drupal.org/project/votingapi), and comms 
(http://rnc08coms.hackbloc.org/download).

- Download and unpack the ARC90 PHP Twitter API 
(http://lab.arc90.com/2008/06/php_twitter_api_client.php or 
http://rnc08coms.hackbloc.org/download) into the comms directory. After you do 
this the directory structure should look like
  comms/
    Arc90_Service_Twitter/
      lib/
        Arc90/
          Service/
            TwitterSearch.php
            Twitter.php
            Twitter/
              Exception.php
              Response.php
anything else in the Arc90_Service_Twitter can be deleted if you would like.

- Add a new content type named "Group" of type "group" that is used by og as a
"Group node" (administer > Content management > Content types > Add content type).

- Specify the Comms content type as one that can be used by og as "Standard 
group post (typically only author may edit). No email notification." 
(Administer > Content types > Content management > Content types > edit on the 
"Comms" line).

- Add a vocabulary name "Priority" for "Comms" nodes (Administer > Content 
management > Categories > Add Vocabulary).

- Add at least the following terms in the "Priority" vocabulary: "Emergency", 
"Info", "Warning" (Administer > Content management > Categories > add terms 
on the "Priority" line).

- Configure your cron job (http://drupal.org/cron) to run  at least on the 
minute.  The module is designed to pull content from twitter.com everytime 
the cron job runs.

NOTES
===============================================================================
- There are two views for comms data located respectively at /comms/slim and 
comms/full.  Each view takes the following arguments: minutes since recieved,
vote value, have or have not been dispatched, has or has not been categorized,
and priority level.  For example:
http://<url>/comms/slim/10/2/1/0/Emergency - shows the slim view for all nodes
recieved int he last 10 minutes, with a vote of 2, that have been dispatched, 
have not been categorized, and are marked with an Emergency priority.

- There is a search page with a form that will fill out of the view arguments for
you at comms/search

- Various settings for the module can be configured at Administer > Site
configuration > Comms module settings (TwitterAPI settings is a tab there
titled 'Comms Twitter API settings')



TODO/BUGS/FEATURE REQUESTS
===============================================================================
- see http://rnc08coms.hackbloc.org/node/1


CREDITS
===============================================================================
This module was written by the hackbloc staff <staff@hackbloc.org>, in hopes 
of "smashing the state" at the 2008 Rupublican National Convention.  They are 
probably down by the river smoking weeds as you read this.  
Contributers:
Mesuir Burf Letet <evoltech@hackbloc.org>
Impact!!! <impact@riseup.net>
Frenzy <frenzy@hackbloc.org>
Dixie Flatline <evoltech@hackbloc.org>
AlxCIAda <alxciada@hackbloc.org>
Mark Burdett <mark@indymedia.org>
Juozas Gaigalas <juozasgaigalas@gmail.com>
Rabble <evan@protest.net>
Blaine Cook <romeda@gmail.com>
