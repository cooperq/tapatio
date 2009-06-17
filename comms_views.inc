<?php
// $Id$

/*
 * @file
 * implements all of the views for the comms module
 *
 * TODO: add (D:<time>) next to title if dispatched
 * TODO: building views with views_ui needs to be tested
 * TODO: create support for views2

 */


/**
 *  Implementation of hook_views_arguments()
 *  This will allow the system to pass url arguments that will be handled by 
 *  the comms system.
 */
function comms_views_arguments() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_arguments()");
  }

  $arguments['comms_minutes'] = array(
    'name' => t('Comms: minutes'),
    'help' => t('The time frame (in minutes) to view comms nodes'),
    'handler' => 'comms_handler_arg_minutes'
  );

  $arguments['comms_vote'] = array(
    'name' => t('Comms: vote'),
    'help' => t('The rating to filter Comms nodes by'),
    'handler' => 'comms_handler_arg_vote'
  );
  
  $arguments['comms_dispatch'] = array(
    'name' => t('Comms: dispatch'),
    'help' => t('Weather or not the node has been disatched or not'),
    'handler' => 'comms_handler_arg_dispatch'
  );
  
  $arguments['comms_priority'] = array(
    'name' => t('Comms: priority'),
    'help' => t('The priority assigned to the node'),
    'handler' => 'comms_handler_arg_priority'
  );
  
  $arguments['comms_categorized'] = array(
    'name' => t('Comms: categorized'),
    'help' => t('Wether or not a comms node has been categorized yet'),
    'handler' => 'comms_handler_arg_categorized'
  );
  
  return $arguments;
}

//the handler for the comms_group 
function comms_handler_field_comms_group_nodelink($op, &$query, $a1, $a2 = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_field_comms_group_nodelink()");
  }

  switch ($op) {
    //TODO: how do I implement this? When is it used?
    case 'sort':
      return $fieldinfo;
      break;
    
    //TODO: how do I implement this? When is it used?
    case 'summary':
      return $fieldinfo;
      break;
    
    case 'filter':
      if ($a2 != 'NULL' && strtolower($a2) != 'all') {
        if ($a2 == 0) {
          $query->add_where('{node}.nid not in (select nid from {comms_group_dispatch})');
        }
        else if ($a2 == 1) {
          $query->add_where('{node}.nid in (select nid from {comms_group_dispatch})');
        }
      }
      break;
    
    //TODO: where does created get set from?
    case 'link':
      break;
    
    case 'title':
      if (strtolower($query) == 'all') {
        $return = t('or have not ');
      }
      else if ($query == 0) {
        $return = t('not ');
      }
      else {
        $return = '';
      }  
       return $return;  
      break;
  }
}//comms_handler_field_comms_group_nodelink

//the handler for the comms_dispatched argument
function comms_handler_arg_dispatch($op, &$query, $a1, $a2 = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_arg_dispatch()");
  }
  
  switch ($op) {
    //TODO: how do I implement this? When is it used?
    case 'sort':
      return $fieldinfo;
      break;
    
    //TODO: how do I implement this? When is it used?
    case 'summary':
      return $fieldinfo;
      break;
    
    case 'filter':
      if (!is_NULL($a2) && strcmp(strtolower($a2), 'all') != 0) {
        //the 'not' here is a special situation we are using for "comms/search"
        if ($a2 == 0 || strcmp($a2, 'not') == 0) {
          $query->add_where('{node}.nid not in (select nid from {comms_group_dispatch})');
        }
        else if ($a2 == 1) {
          $query->add_where('{node}.nid in (select nid from {comms_group_dispatch})');
        }
      }
      break;
    
    //TODO: where does created get set from?
    case 'link':
      break;
    
    case 'title':
      if (strtolower($query) === 'all') {
        $return = t('or have not ');
      }
      else if ($query == 0) {
        $return = 'not ';
       //this is a special situation we are using for "comms/search"
      }
      else if (strcmp($query, 'not') == 0) {
        $return = t('not ');
      }
      else {
        $return = '';
      }  
       return $return;  
      break;
  }
}//comms_handler_arg_dispatch

function comms_handler_arg_categorized($op, &$query, $a1, $a2 = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_arg_categorized()");
  }

  switch ($op) {
    //TODO: how do I implement this? When is it used?
    case 'sort':
      return $fieldinfo;
      break;
    
    //TODO: how do I implement this? When is it used?
    case 'summary':
      return $fieldinfo;
      break;
    
    //we want to check here to see if any of the nodes have not been 
    //assigned to groups yet and that they have not been assigned 
    //priorities
    case 'filter':
      if ($a2 === 0 || strcmp($a2, 'not') == 0) { 
        $query->ensure_table('term_node');
        $query->add_where('{term_node}.tid is NULL');
        
      }
      else if ($a2 == 1) {
        $query->ensure_table('term_node');
        $query->add_where('{term_node}.tid is not NULL');
      }
      break;
    
    //TODO: where does created get set from?
    case 'link':
      break;
    
    case 'title':
      if (strtolower($query) == 'all') {
        return t('has or has not been categorized');
      }
      else if ($query == 1) {
        return t('has been categorized');
      }
      else {
        return t('has not been categorized');
      }
      break;
  }
}//comms_handler_arg_categorized

//the handler for the comms_priority argument
function comms_handler_arg_priority($op, &$query, $a1, $a2 = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_arg_priority()");
  }

  switch ($op) {
    //TODO: how do I implement this? When is it used?
    case 'sort':
      return $fieldinfo;
      break;
    
    //TODO: how do I implement this? When is it used?
    case 'summary':
      return $fieldinfo;
      break;
    
    case 'filter':
      if ($a2 != 'NULL' && strtolower($a2) != 'all') {
        $query->ensure_table('comms_priority');
        $query->add_where('{term_data}.name = \'%s\'', $a2);
      }
      break;
    
    //TODO: where does created get set from?
    case 'link':
      break;
    
    case 'title':
      if (strtolower($query) == 'all') {
        return t('any priority');
      }
      else {
        return t('a priority of @value', array('@value' => $query));
      }
      break;
  }
}//comms_handler_arg_priority

function comms_handler_filter_vote_function($op, $filter, $filterinfo, &$query) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_filter_vote_function()");
  }

  $table = $filterinfo['table'];
  $column = $filterinfo['field'];
  $field = "$table.$column";
  $query->ensure_table($table);
  $query->add_where("%s %s '%s' OR %s is NULL", $field, $filter['operator'], $filter['value'], $field);
}//comms_handler_filter_vote_function()

//TODO: test this
function comms_handler_filter_vote($op, $filter, $filterinfo, &$query) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_filter_vote()");
  }

  $table = $filterinfo['table'];
  $column = $filterinfo['field'];
  $field = "$table.$column";
  $query->ensure_table($table);
  $query->add_where("%s %s %d", $field, $filter['operator'], $filter['value']);
}//comms_handler_filter_vote()

//the handler for the comms_vote argument
function comms_handler_arg_vote($op, &$query, $a1, $a2 = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_arg_vote()");
  }

  //can we pass this off to votingapi_handler_filter_value?
  switch ($op) {
    //TODO: how do I implement this? When is it used?
    case 'sort':
      return $fieldinfo;
      break;
    
    //TODO: how do I implement this? When is it used?
    case 'summary':
      return $fieldinfo;
      break;
    
    case 'filter':
      $query->ensure_table('comms_vote');

      //there is a special case here when the value of $a2 is zero
      //this means all nodes which have not recieved a vote yet
      if ($a2 === 0) {
        //we don't need to add function='sum' here because there will
        //be no rows for this content_id if it has not been voted on
        //yet
        $query->add_where('comms_vote.value is NULL');
      }
      else if (strcmp(strtolower($a2), 'all') == 0) {
        $query->add_where('comms_vote.function=\'sum\' OR comms_vote.value is NULL');
      }
      else {
        $query->add_where('comms_vote.function=\'sum\' AND comms_vote.value = %d', $a2);
      }
      break;
    
    //TODO: where does created get set from?
    case 'link':
      break;
    
    case 'title':
      if (strtolower($query) == 'all') {
        return t('any vote');
      }
      else if ($query == 0) {
        return t('no vote'); 
      }
      else {
        return t('a vote of @value', array('@value' => $query)); 
      }
      break;
  }
}//comms_handler_arg_vote

//the handler for the comms_minutes argument
function comms_handler_arg_minutes($op, &$query, $a1, $a2 = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_arg_minutes()");
  }

  switch ($op) {
    case 'sort':
      //views will always be built against nodes right?
      $query->ensure_table('node', TRUE);
      $query->add_field('created', 'node');
      $fieldinfo['field'] = 'created';
      $query->add_orderby('node', 'created', $a1);
      return $fieldinfo;
      break;
    

    case 'summary':
      //views will always be built against nodes right?
      $query->ensure_table('node', TRUE);
      $query->add_field('created', 'node');
      $fieldinfo['field'] = 'created';
      $query->add_orderby('node', 'created', 'ASC');
      return $fieldinfo;
      break;
    
    case 'filter':
      if ($a2 != 'NULL' && strtolower($a2) != 'all')
        $since = time()-($a2*60)-1;
        $query->add_where("node.created > %d", $since);
      break;
    
    //TODO: where does created get set from?
    case 'link':
      if ($query->created)
        return l($query->created, "$arg/$query->created");
      else
        return l('NULL', "$arg/NULL");
      break;
    
    case 'title':
      if (strtolower($query) == 'all' || $query == '') {
        return t("No restriction on the time frame"); 
      }
      else {
        return t("Comms nodes recieved in the last @number minutes", 
          array('@number' => $query));
      }
      break;
  }

}//comms_handler_arg_minutes

function views_handler_comms_field_group_stats_link($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_field_group_edit_link()");
  }

  $data->type = $data->node_type;
  $data->uid = $data->node_uid;
  if (user_access('edit comms data')) {
    $request_ct = 0;
    $request_error_ct = 0;
    $request_per_min = 0;
    $request_per_hour = 0;
    $request_per_day = 0;
    //get the total number of requests for this group
    switch ($GLOBALS['db_type']) {
      case 'mysql':
        case 'mysqli':
          $sql = 'SELECT success, UNIX_TIMESTAMP(call_time) as call_time from {twitter_api_calls} WHERE gid=%d';
          break;
      case 'pgsql':
          $sql = "SELECT success, date_part('epoch', call_time) as call_time from {twitter_api_calls} WHERE gid=%d";
          break;
    }//switch

    if (!$result = db_query($query, $data->nid)) {
      $message = t("Unable to select from the twitter_api_calls table with nid=@nid", array('@nid' => $data->nid));
      watchdog("comms_group_views", $message);
      drupal_set_message($message);
    }
    else if (db_num_rows($result) != 0) {
      while ($row = db_fetch_array($result)) {
        $request_ct++;
        if ($row['success'] == 0) {
          $request_error_ct++;
        }

        //get the total number of failed requests for this group
        if ($row['call_time'] > time()-60) {
          $request_per_min++;
        }
        else if ($row['call_time'] > time()-(60*60)) {
          $request_per_hour++;
        }
        else if ($row['call_time'] > time()-(60*60*24)) {
          $request_per_day++;
        }
      }//while
    }//else if
    
    //create links with stats per interval (min/hour/day)
    $return = '';
    if ($request_ct) {
      $return .= l(
          t("Total: @request_ct", array('@request_ct' => $request_ct)),
          'comms/tapi/'. $data->nid
        );

    }
    else {
      $return .= t("Total: @ct", array('@ct' => $request_ct));
    }
    
    $return .= 
      l(
        t("Failed: @ct", array('@ct' => $request_error_ct)),
        'comms/tapi/'. $data->nid .'/failed'
      ); 

    $return .= t("Failed: @ct, Rate(@rm/min, @rh/hour, @rd/day)", 
      array('@ct' => $request_error_ct, '@rm' => $request_per_min, 
        '@rh' => $request_per_hour, '@rd' => $request_per_day));

    return  $return;
  }
  else {
    return $value;
  }
}

function views_handler_comms_tapi_field_success($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_tapi_field_success()");
  }

  return $value ? t('Yes') : t('No');
}

function views_handler_comms_tapi_field_call_time($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_tapi_field_call_time()");
  }

  return $value;
}

function views_handler_comms_tapi_field_message($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_tapi_field_message()");
  }

  return $value ? $value : t('N/A');
}

function views_handler_comms_tapi_field_function_link($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_tapi_field_function_link()");
  }

  return l($value, 
    "http://www.arc90.com/_assets/Arc90_Service_Twitter/docs/Arc90_Service/Twitter/Arc90_Service_Twitter.html".
    '#'. $value);
}

function views_handler_comms_field_group_edit_link($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_field_group_edit_link()");
  }

  $data->type = $data->node_type;
  $data->uid = $data->node_uid;
  if (user_access('edit comms data')) {
   return l($value, "node/$data->nid/edit", NULL, drupal_get_destination());
  }
  else {
    return $value;
  }
}

//this is special in that it incorporates the functionality of the following handlers:
//views_handler_field_nodelink_with_mark
//views_handler_node_edit_destination
function views_handler_comms_field_node_edit_mark_link($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_field_node_edit_mark_link()");
  }

  $data->type = $data->node_type;
  $data->uid = $data->node_uid;
  if (user_access('edit comms data')) {
   return l($value, "node/$data->nid/edit", NULL, drupal_get_destination());
  }
  else {
    return $value;
  }
}

//this handler extends the functionality of
//views_handler_field_since()
function views_handler_comms_field_since($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_field_since()");
  }
  return $value ? 
    t('%time min ago', array( '%time' => floor((time() - $value) / 60))) :
    theme('views_nodate');
}

function views_handler_comms_edit_userpoints_link($fieldinfo, $fielddata, $value, $data) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_edit_userpoints_link()");
  }
  return l(
    $value, 
    'admin/user/userpoints/add/'. $data->users_uid,
      array(),
    'destination=comms/slim');
}

function views_handler_comms_vote_priority($fieldinfo, $fielddata, $value, $data ) { 
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_vote_priority()");
  }
  return $data->comms_vote_name ? $data->comms_vote_name : '';
}

function views_handler_comms_vote_value($fieldinfo, $fielddata, $value, $data ) { 
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_vote_value()");
  }
  return $data->comms_vote_value ? $data->comms_vote_value : 0;
}

function views_handler_comms_vote_sort($op, &$query, $sortinfo = NULL, $sortdata = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_vote_sort()");
  }
  return "comms_vote.value";
}

function views_handler_comms_group_value($fieldinfo, $fielddata, $value, $data ) { 
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_handler_comms_group_value()");
  }
  return $data->description ? $data->description : 0;
}

function views_handler_comms_group_sort($op, &$query, $sortinfo = NULL, $sortdata = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("views_handler_comms_group_sort()");
  }
  return "comms_group.description";
}

function views_handler_comms_tapi_function($fieldinfo, $fielddata, $value, $data ) { 
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("views_handler_comms_tapi_function()");
  }
  return $data->function;
}

function views_handler_comms_tapi_success($fieldinfo, $fielddata, $value, $data ) { 
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("views_handler_comms_tapi_success()");
  }
  return $data->success;
}

function views_handler_comms_tapi_message($fieldinfo, $fielddata, $value, $data ) { 
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("views_handler_comms_tapi_message()");
  }
  return $data->message;
}

function views_handler_comms_tapi_call_time_sort($op, &$query, $sortinfo = NULL, $sortdata = NULL) {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("views_handler_comms_tapi_call_time_sort()");
  }
  return "twitter_api_calls.call_time";
}


//This will provide the default views for the module
//we want to have a tabbed list of default views for comms:
//last x min (this should use an argument)
//dispatched (with vote and time filter)
//not dispatched (with vote and time filter)
//variable by vote (this should use an argument)
//

function comms_views_default_views() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_default_views()");
  }
  // avoid miscellaneous problems by forcing the cache to clear before creating a default view
  views_invalidate_cache();
  
  //some default values for all default views
  $page_empty = t('Your query did not return any values.  Its possible there are no comms nodes, or if you specified url arguments for time frame or vote value that there are no nodes that satisfy those values.');
  $page_header_format = '2';
  $use_pager = TRUE;
  $nodes_per_page = 100;
  $argument = array(
    array(
      'type' => 'comms_minutes',
      'argdefault' => 'All',
      'title' => t('%1'),
      'options' => '',
      ),
    array(
    'type' => 'comms_vote',
      'argdefault' => 'All',
      'title' => t('Comms nodes with %2'),
      'options' => '',
    ),
    array(
      'type' => 'comms_dispatch',
      'argdefault' => 'All',
      'title' => t('Comms nodes that have %3'.'been dispatched'),
      'options' => '',
    ),
    array(
      'type' => 'comms_categorized',
      'argdefault' => 'All',
      'title' => '%4',
      'options' => '0',
    ),
    array(
      'type' => 'taxletter',
      'argdefault' => 'All',
      'title' => t('Comms nodes with %5'),
      'options' => '0',
    ),
  );

  //we arn't going to filter by status = 1 for now, because the
  //workflow won't really entail unpublishing nodes.  
  $filter = array(
      array(
      'tablename' => 'node',
      'field' => 'type',
      'operator' => '=',
      'options' => '',
      'value' => 'comms',
      ),
      array(
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
      ),
  );
  
  $access = array();
  
  $comms_slim = comms_views_default_view_comms_slim();
  $comms_slim->page_empty = $page_empty;
  $comms_slim->page_header_format = $page_header_format;
  $comms_slim->use_pager = $use_pager;
  $comms_slim->nodes_per_page = $nodes_per_page;
  $comms_slim->argument = $argument;
  $comms_slim->filter = $filter;
  $comms_slim->access = $access;
  $views[$comms_slim->name] = $comms_slim;

  $comms_full = comms_views_default_view_comms_full();
  $comms_full->page_empty = $page_empty;
  $comms_full->page_header_format = $page_header_format;
  $comms_full->use_pager = $use_pager;

  //2008-08-26, evoltech: we are turning this down on full pages because
  //we started running into memory errors
  $comms_full->nodes_per_page = 25;
  $comms_full->argument = $argument;
  $comms_full->filter = $filter;
  $comms_full->access = $access;
  $views[$comms_full->name] = $comms_full;

  $comms_tapi = comms_views_default_view_comms_tapi();
  $comms_tapi->page_empty = $page_empty;
  $comms_tapi->page_header_format = $page_header_format;
  $comms_tapi->use_pager = $use_pager;
  $comms_tapi->nodes_per_page = $nodes_per_page;
  $comms_tapi->argument = array();
  $comms_tapi->filter = array();
  $comms_tapi->access = $access;
  $views[$comms_tapi->name] = $comms_tapi;

  $comms_tapidetails = comms_views_default_view_comms_tapidetails();
  $comms_tapidetails->page_empty = $page_empty;
  $comms_tapidetails->page_header_format = $page_header_format;
  $comms_tapidetails->use_pager = $use_pager;
  $comms_tapidetails->nodes_per_page = $nodes_per_page;
  $comms_tapidetails->filter = array();
  $comms_tapidetails->access = $access;
  $views[$comms_tapidetails->name] = $comms_tapidetails;

  return $views;
}


function comms_views_default_view_comms_full() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_default_view_comms_full()");
  }
  global $base_url;
  $view = new stdClass();
  $view->name = 'comms_full';
  $view->description = t('Full comms view');
  $view->page = TRUE;
  $view->page_title = t('All Comms Nodes (Full)');

  $view->page_type = 'node';
  $view->url = 'comms/full';

  //There is an example of this in the views module: modules/views_comment.inc
  //I imagine this is similar to stuff here: http://drupal.org/node/99793
  $view->sort =  array(
    array(
        'tablename' => 'node',
        'field' => 'created',
        'sortorder' => 'DESC',
        'options' => '',
    ),
  );

  $view->field = array();
  $view->requires = array(node);

  return $view;
}

//we want to have a default view for twitter api usage:
//group name (link to page), total requests made(link to breakdown by request), 
//total failures (link to failure messages for group), request rate 
//(min/hour/day)
//Is there a $views->page_footer that we can add data to?
function comms_views_default_view_comms_tapi() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_default_view_comms_tapi()");
  }
  global $base_url;
  $view = new stdClass();
  $view->name = 'comms_tapi';
  $view->description = t('Table view of comms twitter API calls');
  $view->page = TRUE;
  $view->page_title = t('Twitter API Calls');
  $view->page_footer = comms_get_default_tapi_view_page_footer();
  $view->page_footer_format = 2;
  
  $view->page_type = 'table';
  $view->url = 'comms/tapi';

  //TODO: figure out how to use this
  $view->sort = array();

  $view->field = array(
    //the organic groups list here
    //this uses a table alias and I have no ideal if it will work
     array(
      'tablename' => 'og',
      'field' => 'description',
      'label' => t('Groups'),
      'handler' => 'views_handler_comms_field_group_edit_link',
      'sortable' => '0',
     ),
     array(
      'tablename' => 'twitter_api_calls',
      'field' => 'gid',
      'label' => t('Stats<br />Total: #, Failed: #, Rate(#/second, #/minute, #/day)'),
      'handler' => 'views_handler_comms_field_group_stats_link',
      'sortable' => '0',
     ),

  );
  $view->requires = array(node, og, twitter_api_calls);

  return $view;
}

function comms_views_default_view_comms_tapidetails() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_default_view_comms_tapidetails()");
  }
  global $base_url;
  $view = new stdClass();
  $view->name = 'comms_tapidetails';
  $view->description = t('Detailed table view of comms twitter API calls');
  $view->page = TRUE;
  $view->page_title = t('Detailed Twitter API Calls');
  
  $view->page_type = 'table';
  $view->url = 'comms/tapidetails';

  //TODO: figure out how to use this
  $view->sort = array();

  $view->field = array(
    //the organic groups list here
    //this uses a table alias and I have no ideal if it will work
     array(
      'tablename' => 'og',
      'field' => 'description',
      'label' => t('Group'),
      'handler' => 'views_handler_comms_field_group_edit_link',
      'sortable' => '0',
     ),
     array(
      'tablename' => 'twitter_api_calls',
      'field' => 'function',
      'label' => t('Function'),
      'handler' => 'views_handler_comms_tapi_field_function_link',
      'sortable' => '0',
     ),
     array(
      'tablename' => 'twitter_api_calls',
      'field' => 'message',
      'label' => t('Message'),
      //do we need to have a handler for this?
      'handler' => 'views_handler_comms_tapi_field_message',
      'sortable' => '0',
     ),
     array(
      'tablename' => 'twitter_api_calls',
      'field' => 'call_time',
      'label' => t('Call Time'),
      'handler' => 'views_handler_comms_tapi_field_call_time',
      'sortable' => '1',
    ),
     array(
      'tablename' => 'twitter_api_calls',
      'field' => 'success',
      'label' => t('Success?'),
      //do we need to have  ahandler for this?
      'handler' => 'views_handler_comms_tapi_field_success',
      'sortable' => '0',
    ),

  );

  $view->requires = array(node, og, twitter_api_calls);

  return $view;
}

//TODO: add a delete field
//get sorting by vote working
function comms_views_default_view_comms_slim() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_default_view_comms_slim()");
  }
  global $base_url;
  $view = new stdClass();
  $view->name = 'comms_slim';
  $view->description = t('Table view of comms nodes');
  $view->page = TRUE;
  $view->page_title = t('All Comms Nodes (Slim)');
  
  $view->page_type = 'table';
  $view->url = 'comms/slim';

  //TODO: figure out how to use this
  $view->sort = array();

  $view->field = array(
   array(
    'tablename' => 'node',
    'field' => 'title',
    'label' => t('Title'),
    'handler' => 'views_handler_comms_field_node_edit_mark_link',
    'sortable' => '0',
   ),
   
   //we want a handler here that will link directly to the userpoints
   //edit page
   array(
    'tablename' => 'users',
    'field' => 'name',
    'label' => t('Author'),
    'sortable' => '1',
    'handler' => 'views_handler_comms_edit_userpoints_link',
    'sortable' => 1,
  ),

  //the organic groups list here
  //this uses a table alias and I have no ideal if it will work
   array(
    'tablename' => 'og_node_data',
    'field' => 'title',
    'label' => t('Groups'),
    'handler' => 'og_handler_field_nodelink',
    'sortable' => '0',
  ),

  //we going to want something here for the time created as well
  array(
    'tablename' => 'node',
    'field' => 'created',
    'label' => t('Recieved'),
    'handler' => 'views_handler_comms_field_since',
    'sortable' => '1',
    'defaultsort' => 'DESC',
  ),
  
  array(
    'tablename' => 'comms_vote',
    'field' => 'value',
    'label' => t('Vote'),
    'sortable' => '1',
  ),

  //finally we would like a delete field
  array(
    'tablename' => 'node',
    'field' => 'delete',
    'label' => t('Delete'),
    'handler' => 'views_handler_node_delete_destination'
  ),

  );

  //We include the
  //votinapi_cache.function = 'sum' because the query is much slower
  //if we include it in the join clause.  
  $view->filter[] = array(
      'tablename' => 'comms_vote',
      'field' => 'function',
      'operator' => '=',
      'options' => '',
      'value' => 'sum',
  );

  $view->requires = array(node, comms_vote);

  return $view;
}

//describe comms to views
function comms_views_tables() {
  if (variable_get('comms_debuglevel', 0) > 0) {
    drupal_set_message("comms_views_tables()");
  }

  //but this only joins when the type is comms
  //TODO: filter by rows with no sms message?
  $tables['comms_additions'] = array(
    'name' => 'comms_additions',
    'fields' => array(
      'sms_message' => array(
        'name' => 'Comms: SMS Message',
        'help' => t('Display the comms SMS message.'),
        'sortable' => FALSE
      )
    ),

    'join' => array(
      'type' => 'inner',
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'nid'
      )
    )
  );

  //TODO:
  //this only joins when the type is group
  //this should be sortable by dispatch time
  //we should provide logic to dsplay statistics like: dispatches per 
  //min/hour/day
  $tables['comms_group_dispatch'] = array(
    'name' => 'comms_group_dispatch',
    'fields' => array(
      'dispatch_time' => array(
        'name' => t('Comms: Dispatch Time'),
        'help' => t('Display the dispatch time for the node.'),
        'sortable' => FALSE
      )
    )
  );

  //this only joins when the type is group
  $tables['group_twitter_account'] = array(
    'name' => 'group_twitter_account',
    'join' => array(
      'type' => 'inner',
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => 'nid'
    )
  );

  //This may not be associated with any node, or
  //it may be associated with a comms node only or
  //a comms node and a group node
  //Note: this may not be possible to implement correctly, because there is no
  //right join support: http://drupal.org/node/99564#right-join
  $tables['twitter_api_calls'] = array(
    'name' => 'twitter_api_calls',
    'fields' => array(
      'gid' => array(
        'name' => t('Comms TAPI: Group Id'),
        'help' => t('TAPI Group Id.'),
        'sortable' => FALSE,
        'handler' => 'views_handler_comms_group_value',
      ),
      'function' => array(
        'name' => t('Comms TAPI: function'),
        'help' => t('TAPI function called.'),
        'sortable' => FALSE,
        'handler' => 'views_handler_comms_tapi_function',
      ),
      'message' => array(
        'name' => t('Comms TAPI: message'),
        'help' => t('TAPI message'),
        'sortable' => FALSE,
        'handler' => 'views_handler_comms_tapi_message',
      ),
      'success' => array(
        'name' => t('Comms TAPI: success'),
        'help' => t('TAPI success boolean'),
        'sortable' => FALSE,
        'handler' => 'views_handler_comms_tapi_success',
      ),
      'call_time' => array(
        'name' => t('Comms TAPI: call time'),
        'help' => t('TAPI call time'),
        'sortable' => TRUE,
        'handler' => 'views_handler_field_date',
        'sort_handler' => 'views_handler_comms_tapi_call_time_sort'
      )
    ),
    'join' => array(
      'type' => 'inner',
      'left' => array(
        'table' => 'og',
        'field' => 'nid'
      ),
      'right' => array(
        'table' => 'twitter_api_calls',
        'field' => 'gid'
      ),
    )
  );

  $tables['comms_group'] = array(
    'name' => 'og',
    'fields' => array(
      'description' => array(
        'name' => t('Comms: Group Description'),
        'help' => t('Display a comms group.'),
        'sortable' => TRUE,
        'handler' => 'views_handler_comms_group_value',
        'sort_handler' => 'views_handler_comms_group_sort'
      )
    ),
    /*
    'join' => array(
      'type' => 'inner',
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'table' => 'og',
        'field' => 'nid'
      )
    )
     */
  );

  $tables['comms_vote'] = array(
    'name' => 'votingapi_cache',
    'fields' => array(
      'value' => array(
        'name' => t('Comms: Vote Value'),
        'help' => t('Display the comms vote value.'),
        'sortable' => TRUE,
        'handler' => 'views_handler_comms_vote_value',
        'sort_handler' => 'views_handler_comms_vote_sort'
      )
    ),
    'join' => array(
      //'type' => 'inner',
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        //'field' => 'content_id AND {comms_vote}.function=\'sum\''
        'field' => 'content_id'
      )
    ),
    'filters' => array(
      'value' => array(
        'name' => t("Comms rating"),
        'operator' => 'views_handler_operator_eqneq',
        'handler' => 'comms_handler_filter_vote',
        'value-type' => 'string',
        'help' => t('Filter intel by its current rating.'),
      ),
      'function' => array(
        'name' => t("Comms vote function"),
        'operator' => '=',
        'handler' => 'comms_handler_filter_vote_function',
        'value-type' => 'string',
        'value' => array(
          '#type' => 'select',
          '#multiple' => '0',
          '#options' => array(
            'count',
            'sum',
            'average')
          ),
        'help' => t('Filter intel votes by the votingapi function.'),
      )
    )
  );
  
  $tables['comms_priority'] = array(
    'name' => 'term_node',
    'fields' => array(
      'name' => array(
        'name' => t('Comms: Priority'),
        'help' => t('The associated priority of the comms node.'),
        'sortable' => FALSE,
        'handler' => 'views_handler_comms_vote_priority',
      )
    ),
    'join' => array(
      'type' => 'inner',
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'nid inner join {term_data} on {term_data}.tid = {comms_priority}.tid'
      )
    )
  );
  return $tables;
}

//TODO: this will start the section collapsed, but will not re-collapse
//after it has been clicked.
//I can't figure out a better way to include this js file, can you?
//since this function is not called everytime the view is generated
//a simple drupal_add_js() is not sufficient.
function comms_get_default_view_page_header($url) {
  $return = '<script type="text/javascript" '.
    'src="/misc/collapse.js"></script>'.
    '<form><fieldset class="collapsible collapsed">'.
    '<legend>'. t('Filtering help') .'</legend><div>'.
    t('You can refine the search results here by passing arguments for the time frame and vote value in the url.  The format is !base. examples: <br />All nodes in the past 10 minutes: !e1<br />All nodes with a vote of 2: !e2<br />All nodes with a in the past 10 min, with a vote of 3, that have not been dispatched: !e3<br />All nodes with with a vote of 0 (aka no votes), that have been dispatched, with a priority of "Info": !e4', array(
    '!base' => l($base_url ."$url/<time_frame>/<vote>/<dispatched>/<urgency>", $url .'/All/All/All/'),
    '!e1' => l($base_url ."$url/10", $url .'/10'),
    '!e2' => l($base_url ."$url/All/2", $url .'/All/2'),
    '!e3' => l($base_url ."$url/10/3/0", $url .'/10/3/0'),
    '!e4' => l($base_url ."$url/All/0/1/Info", $url .'/All/0/1/Info'))) .
    '</div></fieldset></form>';

  return $return;
}

//TODO: This should all be passed through the views theme engine:
//http://drupal.org/node/180924
//This is an attempt to compensate for the fact that there are no
//right joins
function comms_get_default_tapi_view_page_footer() {
  //select all tapi rows that do not have a group associated with it
  $request_ct = 0;
  $request_error_ct = 0;
  $request_per_min = 0;
  $request_per_hour = 0;
  $request_per_day = 0;
  switch ($GLOBALS['db_type']) {
    case 'mysql':
      case 'mysqli':
        $sql = 'SELECT success, UNIX_TIMESTAMP(call_time) as call_time from {twitter_api_calls} WHERE gid=-1 og gid is NULL';
        break;
    case 'pgsql':
        $sql = "SELECT success, date_part('epoch', call_time) as call_time from {twitter_api_calls} WHERE gid=-1 or gid is NULL";
        break;
  }//switch

  if (!$result = db_query($query)) {
    $message = t("Unable to select from the twitter_api_calls table with gid=-1 or gid is NULL");
    watchdog("comms_group_views", $message);
    drupal_set_message($message);
  }
  else if (db_num_rows($result) != 0) {
    while ($row = db_fetch_array($result)) {
      $request_ct++;
      if ($row['success'] == 0) {
        $request_error_ct++;
      }

      //get the total number of failed requests for this group
      if ($row['call_time'] > time()-60) {
        $request_per_min++;
      }
      else if ($row['call_time'] > time()-(60*60)) {
        $request_per_hour++;
      }
      else if ($row['call_time'] > time()-(60*60*24)) {
        $request_per_day++;
      }
    }//while
  }//else if

  $return = "\n<div class='view view-comms-tapi view-comms-tapi-NA'>\n".
    "\t<div class='view-content view-content-comms-tapi'>\n".
    "\t\t<table>\n".
    "\t\t<tbody>\n".
    "\t\t\t<tr class='odd'>\n".
    "\t\t\t<td class='view-field view-field-comms-group-description'>API calls not associated</td>\n".
    "\t\t\t<td class='view-field view-field-twitter-api-calls-gid'>";
 
  //create links with stats per interval (min/hour/day)
  $return .=  t("Total: @rct, Failed: @rect, Rate(@rpm/min , @rph/hour, @rpd/day)",
    array('@rct' => $request_ct, '@rect' => $request_error_ct,
    '@rpm' => $request_per_min, '@rph' => $request_per_hour, 
    '@rpd' => $request_per_day));

  $return .= "</td>\n".
    "\t\t\t</tr>\n".
    "\t\t</tbody>\n\t\t</table>\n\t</div>\n</div>";

  return $return;
}
