import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';

/**
 * This page will read existing configurations from the data base and list them
 * next to radio button so you can select the configuration you want or create
 * new configuration.  You will also have the option to create a new configuration.
 */
export default function ConfigPage()
{
  const configs = ["Fred", "Robot", "joe", "Add New Configuration"];

  return( <Form>
             <h1>Choose Configuration To Load: </h1>
             { configs.map( config =>
                     ( <Form.Check type='radio'
                                   name="configGroup"
                                   id={config}
                                   label={config} />
                     )
                   )
             }
             <Button>Submit</Button>
          </Form>
        );
}