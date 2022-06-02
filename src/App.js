import logo from './logo.svg';
import {ChakraProvider} from "@chakra-ui/react"
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import MainPage from './Views/main/main';
import './App.css';

function App() {
  return (
    <Router>
      <ChakraProvider>
        <Routes>
          <Route path='/' element={<MainPage/>} />
        </Routes>
      </ChakraProvider>
    </Router>
  );
}

export default App;
