/* Name: Lai Man Hin SID: 1155136167 */

const {BrowserRouter, Routes, Route, Link} = ReactRouterDOM;
const {useMatch, useParams, useLocation} = ReactRouterDOM;

const data = [
    {filename: "cuhk-2013.jpg", year: 2013, remarks: "Sunset over CUHK"},
    {filename: "cuhk-2017.jpg", year: 2017, remarks: "Bird's-eye view of CUHK"},
    {filename: "sci-2013.jpg", year: 2013, remarks: "The CUHK Emblem"},
    {filename: "shb-2013.jpg", year: 2013, remarks: "The Engineering Buildings"},
    {filename: "stream-2009.jpg", year: 2009, remarks: "Nature hidden in the campus"},
    ];

class App extends React.Component {
        render() {
        {/* <> fragment for >1 components */}
        return (
            <>
            <Title name={this.props.name}/>
            <BrowserRouter>
                <ul className="list-group">
                    <LongLink to="/" label="Home" />
                    <LongLink to="/gallery" label="Gallery" />
                    <LongLink to="/slideshow" label="SlideShow" />
                </ul>
                <Routes>
                    <Route path="/" element={<Home/>} />
                    <Route path="/gallery" element={<Gallery/>} />
                    <Route path="/slideshow" element={<Slideshow/>} />
                    <Route path="*" element={<NoMatch/>} />
                </Routes>
            </BrowserRouter>
            </>
        );
    }
}

class Title extends React.Component {
    render() {
        return (
        <header className="bg-warning">
            <h1 className="display-4 text-center">{this.props.name}</h1>
        </header>
        );
    }
}

class Home extends React.Component {
    render () {
        return (
        <div>
            <img src={"diagram.png"} style={{width: "100%"}} />
        </div>
        );
    }
}
class Gallery extends React.Component {
    render() {
        return (
            <main className="container">
                {data.map((file,index) => <FileCard i={index} key={index}/>)}
            </main>
        );
    }
}

class Slideshow extends React.Component {
    constructor(props) {
        super(props);
        this.state = {currentImageID: 0, currentInterval: 1500, running: null};
    }

    start = () => {
        if (this.state.running == null)
            this.state.running = setInterval(() => {
                if (this.state.currentImageID == 4)
                    this.setState({currentImageID: 0, currentInterval: this.state.currentInterval});
                else
                    this.setState({currentImageID: this.state.currentImageID + 1, currentInterval: this.state.currentInterval});
            
            }, this.state.currentInterval);
    }

    stop = () => {
        clearInterval(this.state.running);
        this.state.running = null;
    }

    faster = () => {
        if (this.state.currentInterval > 250)
        {
            this.setState({currentInterval: this.state.currentInterval - 250});
        }
    }

    slower = () => {
        this.setState({currentInterval: this.state.currentInterval + 250});
    }

    componentDidUpdate(prevProps, prevStates) {
        if (this.state.running != null) {
            if (this.state.currentInterval != prevStates.currentInterval)
            {
                this.stop();
                this.start();
            }
        }    
    }
    render() {
        return (
            <main className="container mt-4">
                <button className="btn btn-primary" onClick={this.start}>Start Slideshow</button> <button className="btn btn-primary" onClick={this.stop}>Stop Slideshow</button> <button className="btn btn-primary" onClick={this.faster}>Faster</button> <button className="btn btn-primary" onClick={this.slower}>Slower</button>
                <div className="container mt-2">
                    <img src={"images/"+data[this.state.currentImageID].filename}/>
                    <p>{data[this.state.currentImageID].filename}</p>
                </div>
            </main>
        );
    }
}

class FileCard extends React.Component{

    constructor(props) {
        super(props);
        this.state = { selected: -1 };
        {/* this syntax should only be used
        in the constructor, and otherwise
        this.setState() must be used */}
    }

    handleMouseOver(index, e) {
        this.setState({selected: index});
    }

    handleMouseOut(index, e) {
        this.setState({selected: -1});
    }
    render(){
        let i = this.props.i;
        return(
            <div className="card d-inline-block m-2" style={{width:this.state.selected==i ? 220 : 200}} onMouseOver={(e) => this.handleMouseOver(i, e)} onMouseOut={(e) => this.handleMouseOut(i, e)}>
                <img src={"images/"+data[i].filename} className="w-100" />
                <div className="card-body">
                    <h6 className="card-title"> {data[i].filename}</h6>
                    <p className="card-text"> {data[i].year}</p>
                    { this.state.selected==i && <p className="card-text">{data[i].remarks}</p> }
                </div>
            </div>
        )
    }
}

function LongLink({label, to}) {
    let match = useMatch({
        path : to
    });
    return (
        <li className={match ? "list-group-item list-group-item-success": "list-group-item"}>
            {match && "> "}
            <Link to={to}> {label} </Link>
        </li>
    );
}

function NoMatch(){
    let location = useLocation();
    return (
        <div>
            <h3>
                No match for <code>{location.pathname}</code>
            </h3>
        </div>
    );
}

ReactDOM.render(
    <App name="CUHK Pictures"/>,
    document.querySelector("#app"));