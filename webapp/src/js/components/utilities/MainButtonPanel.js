import React, { Component } from "react";
import ConfigButton from "./ConfigButton";
import PlayButton from "./PlayButton";
import '../../../css/MainButtonPanel.css';

/**
 * A panel for holding the buttons on the main page.
 */
export default class MainButtonPanel extends Component
{
   render()
   {
      return( <div className="main-button-panel">
                <div>
                  <ConfigButton/>
                </div>
                <div>
                  <PlayButton/>
                </div>
              </div> );
   }
}