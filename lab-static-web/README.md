<!-- <meta http-equiv="refresh" content="3" > -->
# The (mostly) static web: HTML, CSS, and a little PHP

## HTML

### "Hypertext Markup Language"

HTML is a **markup** language for the web. 

At their most basic level, computers have always represented text as **plain
text** - that is, with no embellishments like bold or italic characters, images,
headings, colors, etc. We refer to text with these extra trappings as **rich
text**. To write rich text, you need some way to annotate your plain text with
indications of which bells and whistles should go where. These annotation
languages are called markup languages.

There are a lot of markup languages out there:

- HTML, the standard markup language of the web, which includes not just rich
  text capabilities but also structural ones
- Markdown, which is for creating rich text only, and is much easier to read and
  write than HTML
- reStructuredText, similar to markdown in purpose and usability,
- [Many others](https://en.wikipedia.org/wiki/Markup_language) 

The [Mozilla developer network's HTML reference](https://developer.mozilla.org/en-US/docs/Web/HTML/Element) is widely regarded as the most useful standard reference for HTML.

### Structure of an HTML Document

Different [user agents](http://www.user-agents.org/) read web pages differently.
It's important to follow web standards as closely as possible while you write
your page, so that all user agents can display your content optimally. Even if
your page looks the same in all major desktop browsers, it may not contain
enough information to be parsed or rendered correctly in mobile browsers, screen
readers, text-only browsers, or by bots or scripts.

That said, here's the bare minimum you need on an html page:

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Page title goes here</title>
  </head>
  <body>
    <h1>The most important heading</h1>

    <p>Maecenas vestibulum mollis diam. Cras dapibus. Vestibulum ullamcorper mauris at ligula. Nullam sagittis. Phasellus blandit leo ut odio.</p>
    
    <p>Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Vivamus aliquet elit ac nisl. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Quisque id odio. Nullam tincidunt adipiscing enim.</p>
    
    <h2>A less important heading</h2>
    
    <p>Quisque id odio. Ut non enim eleifend felis pretium feugiat. Suspendisse eu ligula. Quisque libero metus, condimentum nec, tempor a, commodo mollis, magna. Proin pretium, leo ac pellentesque mollis, felis nunc ultrices eros, sed gravida augue augue mollis justo.</p>
    
  </body>
</html>
```

What do we have here?

0. `<!doctype html>` is the document type declaration (DTD). This lets browsers
   know what version of HTML you are using, and it requires a bit of history
   discussion: 

   In the 1960s, IBM developed the Generalized Markup Language (GML) which was the original specification for a formally-defined document type with a nested element structure. It was used primarily for publishing. Later, over the course of the 1980s, the ISO, ANSI, and several other organizations developeda GML into the SGML (standard generalized markup language) standard. Both XML and (until recently) HTML are implementations of the SGML standard.

   Part of the SGML standard is including a DTD, so that the parser can determine how to parse the document. This became increasingly important as
   the Wordlwide Web Consortium (W3C) continued to release new versions of HTML. The DTD must always appear on the first line of the file.

   When a browser is served a document with the `Content-Type=text/html` header, it looks for a document type declaration to determine how it should render the document. Until recently, they looked something like this:

       <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
          "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

   This gives the name of the specific HTML standard the document is using,
   including a link to full information on the standard. 

   In 2014, HTML5 became the official web standard. HTML5 is *not* an implementation of SGML, and it is the "final" implementation of HTML. This
   makes DTDs obsolete, but we still include one to "blend in" with older web pages. The one true HTML doctype from now on is simply `<!doctype html>`. This gives older browsers some minimal information about your HTML5 document, and lets modern browsers know you're using HTML5.

2. The `<html>` tag encloses the entire document, and its `lang="en"` attribute explicitly specifies the document's langauge. Without this specification, user agents can often guess at the language of a document, but this way there is no guesswork involved.

3. `<head>` and `<body>` tags are for meta information and content, respectively. The head section contains links to supporting files (CSS, javascript, etc) that page needs and meta tags that provide more information about the content, and it is not displayed visually in the browser. The body section contains the visible part of the page.

1. `<meta charset="utf-8">` is hopefully self-explanatory. It tells the user agent which characters set to use when rendering the document.

1. The `<title>` tag supplies, among other things, the title of the browser tab or window when viewing this page. Always fill this in.

1. The `<p>...</p>`, `<h1>...</h1>`, and `<h2>...</h2>` tags, which enclose actual page content and provide some meta information about how to display it. `<p>` encloses a regular paragraph of text, while `<h1>`, `<h2>`, all the way down to `<h6>` signify headings in order of importance (level 1 is the most important and by defalt the largest - MS Word and other word processors also have heading and paragrph styles like this).

### Paired and unpaired tags

You'll notice that in our example page we have some tags that open and close, like this:

```html
<body>
    <!-- content goes here -->
</body>
```

...and some tags that stand alone, like this:
```
<meta charset="utf-8">
```

This is simply because some tags are intended to modify other information, while some tags stand alone as their own chunk of information. 

The meta tag is one declaration about content - it doesn't need to wrap anything.

The body tag, on the other hand, is intended to mark _other_ content as the body of the document, not to convey any information on its own. 

Other examples:

- `<br>` stands alone, and produces a horizontal dividing line wherever it is placed
- `<img>` stands alone, and inserts an image wherever it is placed

- `<p>...</p>` encloses a paragraph of text or other content
- `<a>...</a>` encloses content (text, image, etc) that should link somewhere

### Tags with attributes

Sometimes a single tag alone isn't enough to do anything meaningful. An example is the anchor tag, which is used to make links:

```html
<a>Wikipedia</a>
```

This will display the "Wikipedia" text as a link, but the link won't go anywhere. In most browsers it will just reload the current page. We have to specify at least one more attribute to give it enough information to make a useful link:

```html
<a href="https://en.wikipedia.org/wiki/Main_Page" target="_blank" title="The free encyclopedia">Wikipedia</a>
```
This uses the `href` attribute to specify the destination of the link, and it also adds a `target` attribute that will make the link open in a new tab or window. There's also a `title` attribute that gives more information about the destination.

#### Global attributes

Some attributes, like the `href` above, are used only for one or a few tags, but there are some **global attributes** that can be applied to any tag on a page. [There's a full list here](https://developer.mozilla.org/en-US/docs/Web/HTML/Global_attributes), but perhaps the most important are **`id`** and **`class`**. These attributes provide no meaning on their own, bu they are used to identify certain families of elements or unique elements inside CSS, Javascript, and most HTML parsing utilities. An `id` attribute should only be applied to one element on the page, e.g. `<div id="main-content">...</div>`. A `class` attribute can be applied ot any number of elements, e.g. `<h1 class="light-green">...</h1>` and `<p class="light-green">...</p>` could co-exist on the same page.

### Block and inline tags

Some tags, like `<p>` and `<div>`, will (in most browsers) section themselves off from the rest of the page content with a bit of padding by default. These are called **block level** tags. 

Others, like `<strong>`, `<img>`, and `<span>`, do not interrupt the spacing of the document at all. These are called **inline** tags.

(This behavior can be modified with CSS, but it's useful to know these two words when reading documentation)

### The Document Object Model (DOM)

If you take a look at the structure of our HTML document, you can see how we might look at the nested tags as one large object. If we were writing Java we could almost think about accessing parts of the page like this:

html.body.h1 => this would return all the heading 1s from the document
html.head.title => this would return the title of the document
    
This concept is called the "Document Object Model", or "DOM", and thinking of markup in this way is central to CSS, Javascript, and any HTML parsing library. You don't need to think about this much at the moment other than remembering that it exists.

### Differences between HTML5 and older versions of HTML

#### Allowing for page structure

Originally, HTML was mostly intended to be used to display rich text only, much the way we use markdown now. As the web became a more visual place, we needed not only a way to mark up content, but a way to **structure** that content.

This gave rise to people using the `<table>` tag to emulate a sort of grid system for their page content. But because this was not the intended use of the `<table>` tag, people found that layouts created with tables were both difficult to read and difficult to modify.

Eventually people progressed to using `<div>` tags to specify arbitrary sections of the page that had no inherent meaning in terms of visual display, but could be manipulated to float around the page to start approximating a real grid system. This is still essentially the approach we use today, but the details of the implementation have changed steadily over the years.

These days, unless you're a frontend developer writing styles for a pretty customized site, you're more likely to drop in a CSS library to supply your `<div>` (and other) tags with visual structure, instead of trying to reinvent the wheel yourself. A very simple example is the [Skeleton framework](http://getskeleton.com/), but a web search for "css grid" or "css boilerplate" will return hundreds of possibilities. On Intranet we use Twitter's free [Bootstrap](http://getbootstrap.com/) library to supply our content with both structure and style.


#### Introducing semantic tags and leaving styles in stylesheets

The original implementations of HTML had a lot of tags, like `<b>` for bold and `<div>` for page division, that provided information about what they enclosed that was either too specific or not specific enough. In HTML5, tags have been removed and included with the aim of allowing page authors to avoid specifying stylistic information in HTML, instead letting them focus on the semantic meaning of the content (read more on the [Semantic Web here](https://en.wikipedia.org/wiki/Semantic_Web)). The display of styles is left almost completely to CSS. A few examples:

- Previously, we essentially only had `<div>` tags to use to divide our content into sections. With HTML5 we have tags like [`<article>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/article), [`<nav>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/nav), [`<section>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/section), [`<header>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/header) and others that behave roughly the same as a `<div>` by default, but allow us some built-in meta information about what type of content we're displaying.
- Some tags are obsolete or deprecated in HTML5 because they make assumptions about how to display data. For example, we now typically use the `<strong>` tag where we used to use `<b>` for bold. "Strong" conveys a meaning, "Bold" conveys a style. The `<font>` tag is also obsolete - its sole purpose was to provide style information for text, which is something CSS should take care of.
- Many older tag _attributes_ are also obsolete or deprecated in HTML5. For example, it is possible to specify the alignment of text in a paragraph via the `align` attribute (e.g. `<p align="center">...</p>`), but this is deprecated in HTML5 in favor of using a CSS style to convey the visual information.


## HTML tasks

- Divide your page content into meaningful sections with the [content sectioning tags described on this page](https://developer.mozilla.org/en-US/docs/Web/HTML/Element#Content_sectioning)
- Add a `<nav>` tag and include some [links](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/a) in it
- Add some [images](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/img)
- Get an embed link from a youtube video, put it on your page, and compare it to the [iframe spec](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/iframe)
- **[Make a form](https://developer.mozilla.org/en-US/docs/Web/Guide/HTML/Forms)** that accepts at least a name and email address. We'll use this when we learn about PHP later.

# CSS

User agents generally come with bult-in stylesheets of some kind. This is why when looking at an "unstyled" page in a browser we see blue, underlined links and bold headings.

You can supply your own CSS styles in one of three ways:

1. Include a .css file inside the `<head>` of the page using the `<link>` tag, like this: `<link rel="stylesheet" type="text/css" href="myStyles.css">`
2. Include the styles inside the page itself, by placing them inside a `<style>` tag, e.g. including  `<style> strong { color: green; } h1 { color: #abcdef; }</style>` somewhere in your html would apply that css rule, making all the contents of `<strong>` tags green, and all heading 1s the color signified by the hex code `#abcdef`.
3. Include the `style` attribute on any tag. This allows you to specify styles _only_ for the tag it's applied to, for example, `<p style="font-weight: bold;">...</p>` would make the contents of that specific paragraph bold. 

In CSS, we specify styles for tags using [**selectors**](https://developer.mozilla.org/en-US/docs/Web/Guide/CSS/Getting_started/Selectors), which are hierarchical lists of tag names and other identifiers that can be used to single out what you want to style. Here are some examples to get started with:

```css
h1, h2 {
    color: blueviolet;
}
```
This would make all `<h1>`, `<h2>`, and `<h3>` tags blueviolet.

```css
h1 h2 {
    color: blueviolet;
}
```
By contrast, this would make _only_ `<h2>`s that are inside `<h1>`s blueviolet.

```css
h1.off-black {
    color: #333333;
}
```
This would make only `<h1 class="off-black">` tags dark grey

```css
.off-black {
    color: #333333;
}
```
This would make _any_ tag with `class="off-black"` applied to it dark grey 

```css
div#errors {
    color: red;
}
```
This would make all the contents of the `<div id="errors">` element have red text.

```css
#errors {
    color: red;
}
```
This would do the same, but it would apply to any type of element with the `id="errors"` attribute.

There are lots of other ways to write selectors - all are documented on the Mozilla developer network site.

## CSS tasks
Use [this guide](https://developer.mozilla.org/en-US/docs/Web/Guide/CSS/Getting_Started/) to accomplish the following:

- Add the bootstrap library's css to your page via a `<link>` tag and [this CDN](https://cdnjs.com/libraries/twitter-bootstrap).
- Use bootstrap classes to give your page some style
- Write some overrides to customize the styles based on 1) certain tags, 2) certain classes, and 3) certain id attributes

# Basic PHP

## Getting PHP onto the page

As long as your server knows how to interpret PHP, you can insert PHP on an HTML page anywhere you like, as long as it's inside some `<?php ... ?>` tags. For example, this is completely valid:

```html
...
<h1><?php echo $someVariable; ?></h1>
<p>Other text here</p>
...
```
This would place the contents of the `$someVariable` variable inside the `<h1>` tag and display it to the user as though it were hard-coded onto the page.

Likewise, you can just drop HTML into a PHP file. If there's any PHP at all on a page, it's better to use the `.php` extension instead of the `.html` extension, because not all servers know how to render the PHP parts of the file.


## PHP tasks
- Rename your page from index.html to index.php.
- Echo some values from PHP onto the page
- Print the contents of your HTML form to the page after you submit the form

