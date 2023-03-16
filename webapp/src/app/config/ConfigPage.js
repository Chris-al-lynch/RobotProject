import REACT, { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import Button from 'react-bootstrap/Button';
import Form from 'react-bootstrap/Form';

import DeleteConfirmation from '../components/popups/DeleteConfirmation';
//import { GetAllConfigNames, DeleteConfig } from '../../database/Database';

/**
 * This page will read existing configurations from the data base and list them
 * next to radio button so you can select the configuration you want or create
 * new configuration.  You will also have the option to create a new configuration.
 */
export default function ConfigPage()
{
  const [configs, setConfigs] = useState( [] );
  const [selected, setSelected] = useState( "" );
  const [pickedOne, setPickedOne] = useState( false );
  const [showDeleteConfirm, setShowDeleteConfirm] = useState( false );
  const [itemToDelete, setItemToDelete] = useState( "" );

  //const names = GetAllConfigNames();

  useEffect( () => {
                     const names = ["fred", "joe", "robot", "Iron Giant"];
                     setConfigs( names );
                   }, [] );

  const navigate = useNavigate();

  function onChange( event )
  {
     console.log( 'event.target = ' + event.target.id );
     setSelected( event.target.id );
     setPickedOne( true );
  }

  function onSelect()
  {
     let myText = "You selected " + selected;
     console.log( myText );
     if( selected === "add" )
     {
        navigate( "/configform" );
     }
  }

  function onDelete()
  {
     let myText = "You chose to delete " + selected;
     setItemToDelete( selected );
     setShowDeleteConfirm( true );
     console.log( myText );
  }

  function handleDelete( item )
  {
     //DeleteConfig( item );
     //setConfigs( GetAllConfigNames() );
     setShowDeleteConfirm( false );
  }

  function handleDeleteConfirmationClose()
  {
     setShowDeleteConfirm( false );
  }

  return( <>
            <Form>
              <h1 className='text-light'>Choose Configuration To Load: </h1>
              <div className='text-light' onChange={onChange}>
                { configs.map( config =>
                        ( <Form.Check type='radio'
                                      name='configGroup'
                                      id={config}
                                      key={config}
                                      label={config} />
                        )
                      )
                }
                <Form.Check type='radio'
                            name='configGroup'
                            id='add'
                            key='add'
                            label='Add New Configuration' />
              </div>
              <p> </p>
              {pickedOne ? 
                (<Button onClick={onSelect}>Select</Button>) :
                (<Button disabled>Select</Button>) }
              {pickedOne && (selected !== "add") ?
                (<Button onClick={() => onDelete()}>Delete</Button>) :
                (<Button disabled>Delete</Button>) }
            </Form>
            <div>
              <DeleteConfirmation item={itemToDelete}
                                  showPopup={showDeleteConfirm}
                                  handleDelete={handleDelete}
                                  handleClose={handleDeleteConfirmationClose}
                />
            </div>
          </>
        );
}