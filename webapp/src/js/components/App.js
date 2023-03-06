import React, { Component } from 'react';
import MainButtonPanel from './utilities/MainButtonPanel';
import '../../css/App.css';

/**
 * The main application.  This is the entry into the project.  It contains
 * a button panel which has 2 buttons.
 */
class App extends Component
{
  render()
  {
      return( 
        <div className="App">
             <header className="App-header">Robot Project</header>
             <MainButtonPanel />
        </div>
         );
  }
}

export default App;
