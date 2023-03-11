
import React, { Component } from 'react';
import { Route, Switch } from 'react-router-dom';

import ConfigPage from '../config/ConfigPage';
import PlayPage from '../play/PlayPage';

export default class Router extends Component
{
   getSwitch()
   {
      return( <Switch>
                 <Route exact path='/' component={App} />
                 <Route path='/ConfigPage' component={ConfigPage} />
                 <Route path='/PlayPage' component={PlayPage} />
              </Switch> );
   }

}