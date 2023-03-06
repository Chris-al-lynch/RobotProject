import React, { Component } from 'react';
import '../../../css/OvalButton.css';

/**
 * Component for rendering a pretty oval button.
 * 
 * TODO: Figure out how to vary the color of the button programatically.
 */
export default class OvalButton extends Component
{
   render()
   {
     return(
        <button className="btn btn-oval" onClick={() => this.props.onClick()}>
           {this.props.value}
        </button> );
   }
}