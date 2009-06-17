/* this is a bunch of jquery stuff so that we can submit comms votes without 
 * reloading the page */

//TODO: fix the error handler: commsVoteError()
//TODO: fix commsVoteHook() to correctly use commas

/**
 * Create a degradeable voting interface out of a simple form structure.
 * Returns a modified jQuery object containing the new interface.
 *   
 * @example jQuery('form.rating').rating();
 * @cat plugin
 * @type jQuery 
 *
 */
(function($){ // Create local scope.
	/**
	 * Takes the form element, builds the rating interface and attaches the proper events.
	 * @param {Object} $obj
	 */
	var buildCommsVote = function($obj){	

		var $widget = buildCommsInterface($obj),
			 $comms_votes = $('.comms-vote', $widget);

		// add markup to container and apply click handlers to anchors
		$comms_votes.click(function(){
			
			currentValue = $('select option', $obj).get($comms_votes.index(this)).value;
         
			// Save the currentValue to the hidden select field.
         $("select", $obj).val(currentValue);
         
         // Submit the form if needed.
         $("input.comms-vote-path", $obj).each(function () { 
				$.ajax({ type: 'GET', dataType: 'xml', 
					url: this.value + '/' + currentValue, 
					error: commsVoteError,
					success: commsVoteHook }); 
			});
         return false;
     });//click handler for comms_vote links
	};//buildCommsVote

	//this doesn't work!
	var commsVoteError = function(XMLHttpRequest, textStatus, errorThrown) {
		alert(
				'There was an error processing your vote: ' +
				'errorThrown: ' + errorThrown.text() +'.');
	};

	var commsVoteHook = function(data) {
		//set up a usefull variable
		var xml = {
			message: $("user_message", data).text(),
			voteid: $("vote > id", data).text(),
			sum: $('result summary sum', data).text(),
		};

		//first we clear that div
		$('div#comms-vote-summary-'+xml.voteid).html();

		//if we have a user message then we want to add it
		if (xml.message.length > 0) {
			$('div#comms-vote-summary-'+xml.voteid).html(
				"<label>Rating:</label> <div class=\"value\">"+xml.sum+"</div>"+
				"<label>Error:</label> <div class=\"error\">"+xml.message+"</div>");
		}//if
		else {
			//then we populate it
			$('div#comms-vote-summary-'+xml.voteid).html(
				"<label>Rating:</label> <div class=\"value\">"+xml.sum+"</div>");
		}

	}

	var buildCommsInterface = function($widget) {
		/* $container is a element created on the fly:
		 * http://docs.jquery.com/Core/jQuery#html */
		var $container = $('<div class="comms-vote-widget clear-block"></div>');

		/* Then we select all the option tags in the form */
		var $options = $("select option", $widget);

		/* a count of the number of elements in $options? */
		var size = $('option', $widget).size() - 1;

     	for (var i = 0, option; option = $options[i]; i++){
			/* current count of options we have processed */
		  	var count = i;

			/* build the div that replaces the option */
			$div = $('<div class="comms-vote comms-vote-' + 
					count + 
					'"><a href="#' + 
					option.value + 
					'"><img align="right" src="' + 
					option.text + 
					'"></a></div>');

			$container.append($div[0]);
     	}/* for */
		
		$container.addClass('comms-vote-widget-' + (size - 1));
		
		// Attach the new widget and hide the existing widget.
		$('select', $widget).after($container).hide();

		return $container;
	};//buildCommsInterface

	/**
     * Set up the plugin
     */
    $.fn.comms_vote = function() {
      var stack = [];
      this.each(function() {
          var ret = buildCommsVote($(this));
          stack.push(ret);
      });
      return stack;
    };
	
	// Fix ie6 background flicker problem.
  	if ($.browser.msie == true) {
    	try {
      	document.execCommand('BackgroundImageCache', false, true);
    	} catch(err) {}
  	}
})(jQuery);

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('div.comms-form-item').comms_vote();
    /* $('input.fivestar-submit').hide(); */
  });
}
