from django.db import models
from django.contrib.auth.models import User

class CollationImage(models.Model):
    image_id = models.AutoField(primary_key=True)
    path = models.CharField(max_length=1024)
    name = models.CharField(max_length=1024, null=True)
    size = models.IntegerField()

class Result(models.Model):
    result_id = models.AutoField(primary_key=True)
    data = models.TextField() #stored as JSON Object

class CollationPair(models.Model):
    pair_id = models.AutoField(primary_key=True)
    image_template = models.ForeignKey(CollationImage, related_name='image_template', null=True)
    image_target = models.ForeignKey(CollationImage, related_name='image_target', null=True)
    image_transformed = models.ForeignKey(CollationImage, related_name='image_transformed', null=True)
    result = models.ForeignKey(Result, null=True)

    def get_contents(self):
        return "%s , %s" % (self.image_template.name, self.image_target.name)
    contents = property(get_contents)

class CollationMulti(models.Model):
    multi_id = models.AutoField(primary_key=True)
    progress = models.IntegerField()

class CollationBook(models.Model):
    book_id = models.AutoField(primary_key=True)
    progress = models.IntegerField()

class MultiHasPair(models.Model):
    multi = models.ForeignKey(CollationMulti)
    pair = models.ForeignKey(CollationPair)

class BookHasPair(models.Model):
    book = models.ForeignKey(CollationBook)
    pair = models.ForeignKey(CollationPair)
    page_num = models.IntegerField()

class UserProfile(models.Model):
    user = models.ForeignKey(User)
    affiliation = models.CharField(max_length=160)

class UserHasPair(models.Model):
    name = models.CharField(max_length=140)
    user = models.ForeignKey(User)
    pair = models.ForeignKey(CollationPair)

    def get_contents(self):
        return self.pair.contents
    contents = property(get_contents)

class UserHasMulti(models.Model):
    name = models.CharField(max_length=140)
    user = models.ForeignKey(User)
    multi = models.ForeignKey(CollationMulti)

    def get_contents(self):
        contents = []
        for relation in MultiHasPair.objects.filter(multi_id=self.multi.multi_id):
            contents.append({'text':relation.pair.contents, 'id':relation.pair.pair_id})
        return contents
    contents = property(get_contents)

class UserHasBook(models.Model):
    name = models.CharField(max_length=140)
    user = models.ForeignKey(User)
    book = models.ForeignKey(CollationBook, related_name='book_target')

    def get_contents(self):
        contents = []
        for relation in BookHasPair.objects.filter(book_id=self.book.book_id):
            contents.append({'text':relation.pair.contents, 'id':relation.pair.pair_id})
        return contents
    contents = property(get_contents)
